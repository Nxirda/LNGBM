#include <algorithm>
#include <cblas.h>
#include <omp.h>
#include <random>
#include <stack>
#include <tuple>

#include "Histogram.hpp"
#include "HistogramTrainingElement.hpp"
#include "MAE.hpp"

/**************************/
/*                        */
/* TRAINING ELEMENT CLASS */
/*                        */
/**************************/

//
HistogramTrainingElement::HistogramTrainingElement() {
  this->depth = 0;
  this->node = nullptr;
}

//
HistogramTrainingElement::HistogramTrainingElement(TreeNode *node,
                                                   uint16_t depth) {
  this->node = node;
  this->depth = depth;
}

//
HistogramTrainingElement::HistogramTrainingElement(
    const HistogramTrainingElement &TE) {
  this->node = TE.node;
  this->depth = TE.depth;
  this->Histograms = TE.Histograms;
}

//
HistogramTrainingElement::HistogramTrainingElement(
    HistogramTrainingElement &&TE) {
  this->node = std::move(TE.node);
  this->depth = TE.depth;
  this->Histograms = std::move(TE.Histograms);
}

HistogramTrainingElement &
HistogramTrainingElement::operator=(HistogramTrainingElement &&TE) {
  this->node = std::move(TE.node);
  this->depth = TE.depth;
  this->Histograms = std::move(TE.Histograms);
  return *this;
}

//
HistogramTrainingElement &
HistogramTrainingElement::operator=(const HistogramTrainingElement &TE) {
  this->node = TE.node;
  this->depth = TE.depth;
  this->Histograms = TE.Histograms;
  return *this;
}

//
HistogramTrainingElement::~HistogramTrainingElement(){};

//
void HistogramTrainingElement::set_Node(TreeNode *node) { this->node = node; }

//
void HistogramTrainingElement::set_depth(uint16_t depth) {
  this->depth = depth;
}

//
std::vector<size_t> HistogramTrainingElement::bootstrap(size_t dataset_Size) {

  // Generate a unique seed using hardware entropy
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_int_distribution<size_t> dist(0, dataset_Size - 1);
  std::vector<size_t> index(dataset_Size);

  for (size_t i = 0; i < dataset_Size; ++i) {
    index[i] = dist(gen);
  }

  return std::move(index);
}

//
double compute_Gradient(double actual, double predicted) {
  return predicted - actual;
}

//
void HistogramTrainingElement::fill_Histograms(
    const DataSet &data, const std::vector<size_t> &index) {

  size_t number_Of_Features = data.features_Number();
  size_t number_Of_Samples = data.samples_Number();

  const std::vector<double> &labels = data.get_Labels();

  double label_Mean =
      cblas_dasum(static_cast<int>(number_Of_Samples), labels.data(), 1);
  label_Mean *= (1.0 / number_Of_Samples);

  for (size_t feature = 0; feature < number_Of_Features; ++feature) {
    const std::vector<double> &values = data.get_Column(feature);
    for (auto &sample_Index : index) {
      double sample_Value = values[sample_Index];
      double gradient = compute_Gradient(labels[sample_Index], label_Mean);
      this->Histograms[feature].add_Point(sample_Value, gradient);
    }
  }
}

//
void HistogramTrainingElement::init_Histograms(const DataSet &data) {
  std::vector<size_t> index = std::move(bootstrap(data.samples_Number()));

  for (size_t i = 0; i < data.features_Number(); ++i) {
    Histogram2 h(256, data.get_Column(i), index);
    this->Histograms[i] = std::move(h);
  }

  fill_Histograms(data, index);
}

//
void HistogramTrainingElement::set_Root(TreeNode *node) {
  this->depth = 0;
  this->node = node;
}

//
std::tuple<Histogram2, Histogram2>
HistogramTrainingElement::split_Histogram(size_t feature) const {}

//
double
HistogramTrainingElement::compute_Split_Value(size_t bin_Index, size_t feature,
                                              const IOperator *op) const {

  const Histogram2 &histogram = this->Histograms.at(feature);
  size_t number_Of_Bins = histogram.get_Number_Of_Bins();

  double gradient_Sum_Per_Bin;
  std::vector<double> gradient_Sums_Left(bin_Index);
  double left_Population = 0;
  std::vector<double> gradient_Sums_Right(number_Of_Bins - bin_Index);
  double right_Population = 0;

  size_t bin;
  double gradient;
  double base_Population;

  // Two for just to avoid branching
  for (bin = 0; bin < bin_Index; ++bin) {
    const Bin &curr_Bin = histogram.get_Bins()[bin];
    gradient = curr_Bin.get_Statistic();

    gradient_Sum_Per_Bin += gradient;
    base_Population += curr_Bin.get_Count();

    left_Population += curr_Bin.get_Count();
    gradient_Sums_Left[bin] = gradient;
  }

  for (bin = bin_Index; bin < number_Of_Bins; ++bin) {
    const Bin &curr_Bin = histogram.get_Bins()[bin];
    gradient = curr_Bin.get_Statistic();

    gradient_Sum_Per_Bin += gradient;
    base_Population += curr_Bin.get_Count();

    right_Population += curr_Bin.get_Count();
    gradient_Sums_Right[bin - bin_Index] = gradient;
  }

  double metric_Result_Left =
      op->compute(gradient_Sums_Left, gradient_Sum_Per_Bin);
  // op->compute(left_Labels.value(), left_Prediction);
  double metric_Result_Right =
      op->compute(gradient_Sums_Right, gradient_Sum_Per_Bin);
  // op->compute(right_Labels.value(), right_Prediction);

  // Compute the result of the metric for the split at position
  double res = ((metric_Result_Left * left_Population) +
                (metric_Result_Right * right_Population));

  res *= (1.0 / base_Population);

  return res;
}

//
std::tuple<double, double, size_t>
HistogramTrainingElement::best_Histogram_Split(size_t feature) const {
  const Histogram2 &histogram = this->Histograms.at(feature);

  for (size_t bin = 0; bin < histogram.get_Number_Of_Bins(); ++bin) {
    double split_Score = compute_Split_Value(bin, feature, operator);

    if (split_Score < std::get<0>(candidates[thread_Id])) {
      candidates[thread_Id] = {split_Score, spliting_Threshold, column};
    }
  }
}

//
std::tuple<size_t, double> HistogramTrainingElement::find_Best_Split(
    const IOperator *splitting_Operator) const {
  this->Histograms;
  std::vector<std::tuple<double, double, size_t>> candidates;

  for (size_t idx_H = 0; idx_H < this->Histograms.size(); ++idx_H) {
    candidates[idx_H] = std::move(best_Histogram_Split(idx_H));
  }

  std::tuple<double, double, size_t> best_Split = {-1.0, -1.0, 0};
  for (const auto &candidate : candidates) {
    const auto &best = std::get<0>(best_Split);
    if (best > std::get<0>(candidate) || best == -1.0) {
      best_Split = std::move(candidate);
    }
  }

  return {std::get<2>(best_Split), std::get<1>(best_Split)};
}

//
std::tuple<std::optional<HistogramTrainingElement>,
           std::optional<HistogramTrainingElement>>
HistogramTrainingElement::split_Node(const DataSet &data,
                                     const IOperator *splitting_Operator) {
  // Left node
  std::optional<HistogramTrainingElement> train_Left = std::nullopt;

  // Right node
  std::optional<HistogramTrainingElement> train_Right = std::nullopt;

  // Compute split attributes
  auto [column, criterion] = find_Best_Split(splitting_Operator);

  // Compute new Bins
  auto [left_Bin, right_Bin] = split_Bin(column, criterion);
  // split_Index(data.get_Column(column), this->get_Index(), criterion);

  if (!left_Bin && !right_Bin) {
    return {train_Left, train_Right};
  }

  uint16_t next_Depth = this->depth + 1;

  // Set the datas for the current node
  this->node->set_Split_Column(column);
  this->node->set_Split_Criterion(criterion);

  // Case 1 : Build Right Node (if information gained)
  /* if (right_Bin.has_value()) {
    double predic_Right = mean_Vector_At_Index(data.get_Labels(), *right_index);
    TreeNode right{};
    right.set_Predicted_Value(predic_Right);

    this->node->add_Right(std::make_unique<TreeNode>(std::move(right)));
    train_Right = std::move(
        HistogramTrainingElement(this->node->get_Right_Node(), next_Depth));
  }

  // Case 2 : Build Left Node (if information gained)
  if (left_Bin.has_value()) {
    double predic_Left = mean_Vector_At_Index(data.get_Labels(), *left_index);
    TreeNode left{};
    left.set_Predicted_Value(predic_Left);

    this->node->add_Left(std::make_unique<TreeNode>(std::move(left)));
    train_Left = std::move(
        HistogramTrainingElement(this->node->get_Left_Node(), next_Depth));
  } */

  return {train_Left, train_Right};
}

//
/* std::tuple<size_t, double> HistogramTrainingElement::find_Best_Split(
    const DataSet &data, const IOperator *splitting_Operator) const {
  std::vector<std::tuple<double, double, size_t>> candidates;

  std::vector<std::vector<double>> splitting_Thresholds;
  splitting_Thresholds.resize(data.features_Number());

#pragma omp parallel
  {
    size_t column;
    int thread_Id = omp_get_thread_num();

#pragma omp single
    {
      candidates.resize(omp_get_num_threads(),
                        {std::numeric_limits<double>::max(), 0.0, 0});
    }

#pragma omp for schedule(static)
    for (column = 0; column < data.features_Number(); ++column) {
      splitting_Thresholds[column] =
          splitting_Criteria->compute(data.get_Column(column), this->index);
    }

// No wait just not to have two barriers that follows each other
#pragma omp for schedule(static) nowait
    for (column = 0; column < data.features_Number(); ++column) {
      double split_Score;
      for (const auto spliting_Threshold : splitting_Thresholds[column]) {
        split_Score = compute_Split_Value(
            this->index, data, column, spliting_Threshold, splitting_Operator);

        if (split_Score < std::get<0>(candidates[thread_Id])) {
          candidates[thread_Id] = {split_Score, spliting_Threshold, column};
        }
      }
    }
  } // End of pragma omp parallel

  std::tuple<double, double, size_t> best_Split = {-1.0, -1.0, 0};
  for (const auto &candidate : candidates) {
    const auto &best = std::get<0>(best_Split);
    if (best > std::get<0>(candidate) || best == -1.0) {
      best_Split = std::move(candidate);
    }
  }

  return {std::get<2>(best_Split), std::get<1>(best_Split)};
} */

//
void HistogramTrainingElement::train(const DataSet &data, TreeNode *node,
                                     const IOperator *splitting_Operator,
                                     uint16_t max_Depth, size_t threshold) {

  HistogramTrainingElement base_Node(node, max_Depth);

  // Initialize the root Node
  // base_Node.set_Root(data, Node);

  base_Node.init_Histograms(data);

  double base_Prediction = 0;

  base_Node.node->set_Predicted_Value(base_Prediction);

  // Initialize a stack of Nodes that will be splitted
  std::stack<HistogramTrainingElement> remaining;
  remaining.push(base_Node);

  // Build iteratively the tree frame
  while (not remaining.empty()) {
    auto elem = remaining.top();

    remaining.pop();

    if (elem.depth >= max_Depth) {
      continue;
    }

    auto [left, right] = elem.split_Node(data, splitting_Operator);

    if (left) {
      // Verify we gained information
      if (left.value().index.size() != elem.index.size() &&
          left.value().index.size() > threshold) {
        remaining.push(std::move(*left));
      }
    }

    if (right) {
      // Verify we gained information
      if (right.value().index.size() != elem.index.size() &&
          right.value().index.size() > threshold) {
        remaining.push(std::move(*right));
      }
    }
  }
}