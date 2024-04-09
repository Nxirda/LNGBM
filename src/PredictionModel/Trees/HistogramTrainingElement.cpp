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
  this->labels = TE.labels;
}

//
HistogramTrainingElement::HistogramTrainingElement(
    HistogramTrainingElement &&TE) {
  this->node = std::move(TE.node);
  this->depth = TE.depth;
  this->Histograms = std::move(TE.Histograms);
  this->labels = std::move(TE.labels);
}

HistogramTrainingElement &
HistogramTrainingElement::operator=(HistogramTrainingElement &&TE) {
  this->node = std::move(TE.node);
  this->depth = TE.depth;
  this->Histograms = std::move(TE.Histograms);
  this->labels = std::move(TE.labels);
  return *this;
}

//
HistogramTrainingElement &
HistogramTrainingElement::operator=(const HistogramTrainingElement &TE) {
  this->node = TE.node;
  this->depth = TE.depth;
  this->Histograms = TE.Histograms;
  this->labels = TE.labels;
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
std::vector<size_t>
HistogramTrainingElement::bootstrap_Index(size_t dataset_Size) {

  // Generate a unique seed using hardware entropy
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> dist(0, dataset_Size - 1);
  std::vector<size_t> idx(dataset_Size);

  for (size_t i = 0; i < dataset_Size; ++i) {
    idx[i] = dist(gen);
  }

  return std::move(idx);
}

//
void HistogramTrainingElement::set_Root(const DataSet &data, TreeNode *node) {
  this->depth = 0;
  std::vector<size_t> base_Index = bootstrap_Index(data.samples_Number());

  // HAVE TO SET LABELS BOOTSTRAPPED TOO

  for (size_t i = 0; i < data.features_Number(); ++i) {
    Histogram2 h(256, data.get_Column(i), base_Index);
    this->Histograms[i] = std::move(h);
  }

  this->node = node;
}

//
std::tuple<std::optional<std::vector<double>>,
           std::optional<std::vector<double>>>
HistogramTrainingElement::split_Bin(size_t feature, double criterion) {

  const std::vector<double> &test = this->Histograms[feature].get_Histogram();
  std::vector<double> left_Bin_Part;
  std::vector<double> right_Bin_Part;

  for (size_t i = 0; i < test.size(); ++i) {
    double value = test[i];
    if (value < criterion) {
      left_Bin_Part.push_back(value);
    } else {
      right_Bin_Part.push_back(value);
    }
  }

  return {std::move(left_Bin_Part), std::move(right_Bin_Part)};
}

//
double HistogramTrainingElement::compute_Split_Value(
    const std::vector<size_t> &index, const DataSet &data, size_t feature,
    const IOperator *op) const {

  /* auto [left_Labels, right_Labels] = split_Labels(
      data.get_Column(feature), data.get_Labels(), criteria, index); */

  if (!left_Labels && !right_Labels) {
    return -1.0;
  }

  const size_t base_Population = index.size();
  const size_t left_Population = left_Labels.value().size();
  const size_t right_Population = right_Labels.value().size();

  double metric_Result_Left = 0.0;
  double metric_Result_Right = 0.0;

  double left_Prediction =
      cblas_dasum(left_Population, left_Labels.value().data(), 1.0) * 1.0 /
      left_Population;

  double right_Prediction =
      cblas_dasum(right_Population, right_Labels.value().data(), 1.0) * 1.0 /
      right_Population;

  metric_Result_Left = op->compute(left_Labels.value(), left_Prediction);
  metric_Result_Right = op->compute(right_Labels.value(), right_Prediction);

  // Compute the result of the metric for the split at position
  double res = ((metric_Result_Left * left_Population) +
                (metric_Result_Right * right_Population));

  res *= (1.0 / base_Population);

  return res;
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
  auto [column, criterion] = find_Best_Split(data, splitting_Operator);

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
  if (right_Bin.has_value()) {
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
  }

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
float compute_loss_reduction(const HistogramBin &histogram_bin,
                             const std::vector<float> &labels) {
  if (histogram_bin.count == 0) {
    return 0.0f; // Return 0 loss reduction for empty bins
  }
  // Compute the mean label value in the bin
  float mean_label = histogram_bin.sum / histogram_bin.count;
  // Compute the absolute errors for each label in the bin
  float absolute_errors = 0.0f;
  for (const float &label : labels) {
    absolute_errors += std::abs(label - mean_label);
  }
  // Compute the total absolute error
  float total_absolute_error = absolute_errors / histogram_bin.count;
  // Compute the loss reduction (negative of the total absolute error)
  float loss_reduction = -total_absolute_error;
  return loss_reduction;
}

//
std::tuple<size_t, double> HistogramTrainingElement::find_Best_Split(
    const IOperator *splitting_Operator) const {
  this->Histograms;
  std::vector<std::tuple<double, double, size_t>> candidates;

  for (size_t idx_H = 0; idx_H < this->Histograms.size(); ++idx_H) {
    for (size_t bin_index = 0;
         bin_index < this->Histograms.at(idx_H).get_Number_Of_Bins();
         ++bin_index) {

      float loss_Reduction = 0;
    }
  }
}

//
void HistogramTrainingElement::train(const DataSet &data, TreeNode *Node,
                                     const IOperator *splitting_Operator,
                                     uint16_t max_Depth, size_t threshold) {
  HistogramTrainingElement base_Node{};

  // Initialize the root Node
  base_Node.set_Root(data, Node);

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