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
void HistogramTrainingElement::set_Histogram(size_t feature,
                                             Histogram2 &&histogram) {
  if (this->Histograms.size() < feature) {
    std::cerr << "Histogram to set is out of range\n";
    exit(1);
  }
  this->Histograms.at(feature) = std::move(histogram);
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
double compute_Residual(double actual, double predicted) {
  return actual - predicted;
}

//
double mean_Vector_At_Index(const std::vector<double> &vector,
                            const std::vector<size_t> &index) {
  if (index.empty()) {
    return 0.0;
  }

  double mean = 0.0;
  const double length = index.size();

  for (const auto &idx : index) {
    mean += vector[idx];
  }

  mean *= (1.0 / length);
  return (mean);
}

//
void HistogramTrainingElement::fill_Histograms(
    const DataSet &data, const std::vector<size_t> &index) {

  size_t number_Of_Features = data.features_Number();
  size_t number_Of_Samples = data.samples_Number();

  const std::vector<double> &labels = data.get_Labels();

  double label_Mean = mean_Vector_At_Index(labels, index);
  this->node->set_Predicted_Value(label_Mean);

  for (size_t feature = 0; feature < number_Of_Features; ++feature) {
    const std::vector<double> &values = data.get_Column(feature);

    for (auto &sample_Index : index) {

      double sample_Value = values[sample_Index];
      double target = labels[sample_Index];

      double residual = -compute_Residual(target, label_Mean);

      this->Histograms[feature].add_Point(sample_Value, residual, target);
    }
    this->Histograms[feature].clean_Empty_Bins();
  }
}

//
void HistogramTrainingElement::init_Histograms(const DataSet &data) {

  std::vector<size_t> index = std::move(bootstrap(data.samples_Number()));

  for (size_t feature = 0; feature < data.features_Number(); ++feature) {
    Histogram2 h(256, data.get_Column(feature), index);
    this->Histograms[feature] = std::move(h);
  }

  fill_Histograms(data, index);
}

//
void HistogramTrainingElement::set_Root(TreeNode *node) {
  this->depth = 0;
  this->node = node;
}

//
std::tuple<std::optional<Histogram2>, std::optional<Histogram2>>
HistogramTrainingElement::split_Histogram(size_t bin, size_t feature) const {
   std::cout << "We enter histogram split function \n";
   
  const Histogram2 &histogram = this->Histograms.at(feature);
  const std::vector<Bin> &bins = histogram.get_Bins();
  const size_t size = histogram.get_Number_Of_Bins();

  if (bin == 0 || bin == size) {
    return {std::nullopt, std::nullopt};
  }

  std::vector<Bin> left_Bins(bin);
  std::vector<Bin> right_Bins(size - bin + 1);

  size_t curr_Bin;
  for (curr_Bin = 0; curr_Bin < bin; ++curr_Bin) {
    left_Bins[curr_Bin] = bins[curr_Bin];
  }

  for (curr_Bin = bin; curr_Bin < size; ++curr_Bin) {
    right_Bins[size - curr_Bin] = bins[curr_Bin];
  }

  Histogram2 left_Histogram(std::move(left_Bins));
  Histogram2 right_Histogram(std::move(right_Bins));

  return {std::move(left_Histogram), std::move(right_Histogram)};
}

//
double HistogramTrainingElement::compute_Split_Value(size_t bin_Index,
                                                     size_t feature) const {

  const Histogram2 &histogram = this->Histograms.at(feature);
  size_t number_Of_Bins = histogram.get_Number_Of_Bins();

  double residual_Sums_Left = 0;
  double left_Population = 0;

  double residual_Sums_Right = 0;
  double right_Population = 0;

  // Not sure tho
  double hessian_Value = 1.0;

  size_t bin;

  // Two for just to avoid branching
  for (bin = 0; bin < bin_Index; ++bin) {
    const Bin &curr_Bin = histogram.get_Bins()[bin];
    left_Population += curr_Bin.get_Count();

    residual_Sums_Left += curr_Bin.get_Residual_Sum();
  }

  for (bin = bin_Index; bin < number_Of_Bins; ++bin) {
    const Bin &curr_Bin = histogram.get_Bins()[bin];

    right_Population += curr_Bin.get_Count();

    residual_Sums_Right += curr_Bin.get_Residual_Sum();
  }

  double score_Left =
      pow(residual_Sums_Left, 2) * (1.0 / (hessian_Value * left_Population));

  double score_Right =
      pow(residual_Sums_Right, 2) * (1.0 / (hessian_Value * right_Population));

  double res = score_Left + score_Right;

  return res;
}

//
double HistogramTrainingElement::compute_Predicted_Value() const {
  double label_Sum = 0;
  double total_Bins = 0;
  for (size_t feature = 0; feature < this->Histograms.size(); ++feature) {
    for (size_t bin = 0;
         bin < this->Histograms.at(feature).get_Number_Of_Bins(); ++bin) {
      label_Sum += this->Histograms.at(feature).get_Bins()[bin].get_Target();
      /* std::cout << this->Histograms.at(feature).get_Bins()[bin].get_Target()
                << "\n";*/
      total_Bins += 1; 
      // this->Histograms.at(feature).get_Bins()[bin].get_Count();
    }
  }
  // HERE
  std::cout << label_Sum * (1.0 / total_Bins) << "\n";
  return label_Sum * (1.0 / total_Bins);
}

//
std::tuple<double, size_t, size_t>
HistogramTrainingElement::best_Histogram_Split(size_t feature) const {

  /*   std::cout << "We enter best histogram split finding \n";
   */
  const Histogram2 &histogram = this->Histograms.at(feature);

  double best_Score = 0.0;
  std::tuple<double, double, float> candidate;
  // Might start at one
  for (size_t bin = 0; bin < histogram.get_Number_Of_Bins(); ++bin) {
    double split_Score = compute_Split_Value(bin, feature);
    
    //std::cout << "Split score = " << split_Score << std::endl;

    if (split_Score > best_Score) {
      best_Score = split_Score;
      candidate = {split_Score, bin, feature};
    }
  }
  return candidate;
}

//
std::tuple<size_t, size_t> HistogramTrainingElement::find_Best_Split() const {

  std::vector<std::tuple<double, size_t, size_t>> candidates;
  candidates.resize(this->Histograms.size(), {0.0, 0, 0});

  for (size_t idx_H = 0; idx_H < this->Histograms.size(); ++idx_H) {
    candidates[idx_H] = std::move(best_Histogram_Split(idx_H));
    std::cout << "Candidate : " << idx_H;
    std::cout << "| split score : " << std::get<0>(candidates[idx_H]);
    std::cout << "| split bin : " << std::get<1>(candidates[idx_H]);
    std::cout << "| split feature : " << std::get<2>(candidates[idx_H]);
    std::cout << "\n";  
  }

  std::tuple<double, size_t, size_t> best_Split = {-1.0, 0, 0};
  for (const auto &candidate : candidates) {
    const auto &best = std::get<0>(best_Split);
    // Verify sign here
    if (best < std::get<0>(candidate) || best == -1.0) {
      best_Split = std::move(candidate);
    }
  }

  return {std::get<1>(best_Split), std::get<2>(best_Split)};
}

//
std::tuple<std::optional<HistogramTrainingElement>,
           std::optional<HistogramTrainingElement>>
HistogramTrainingElement::split_Node(const DataSet &data) {

  std::cout << "Enter split node\n";
  // Left node
  std::optional<HistogramTrainingElement> train_Left = std::nullopt;

  // Right node
  std::optional<HistogramTrainingElement> train_Right = std::nullopt;

  // Compute split attributes
  auto [bin, feature] = find_Best_Split();

  std::cout << "Best Bin     is " << bin << "\n";
  std::cout << "Best feature is " << feature << "\n";

  // Compute new Histogram
  auto [left_Histogram, right_Histogram] = split_Histogram(bin, feature);

  if (!left_Histogram && !right_Histogram) {
    return {train_Left, train_Right};
  }

  uint16_t next_Depth = this->depth + 1;

  double split_Criterion = this->Histograms[feature].get_Bins()[bin].get_Max();

  // Set the datas for the current node
  this->node->set_Split_Column(feature);
  this->node->set_Split_Criterion(split_Criterion);

  // Case 1 : Build Right Node (if information gained)
  if (right_Histogram.has_value()) {
    TreeNode right{};

    this->node->add_Right(std::make_unique<TreeNode>(std::move(right)));
    HistogramTrainingElement train_Right_Tmp = *this;
    train_Right_Tmp.set_depth(next_Depth);
    train_Right_Tmp.set_Node(this->node->get_Right_Node());
    train_Right_Tmp.set_Histogram(feature, std::move(*right_Histogram));

    train_Right = std::move(train_Right_Tmp);

    double prediction = train_Right.value().compute_Predicted_Value();
    train_Right.value().node->set_Predicted_Value(prediction);
  }

  // Case 2 : Build Left Node(if information gained)
  if (left_Histogram.has_value()) {
    TreeNode left{};

    this->node->add_Left(std::make_unique<TreeNode>(std::move(left)));
    HistogramTrainingElement train_Left_Tmp = *this;
    train_Left_Tmp.set_depth(next_Depth);
    train_Left_Tmp.set_Node(this->node->get_Left_Node());
    train_Left_Tmp.set_Histogram(feature, std::move(*left_Histogram));

    train_Left = std::move(train_Left_Tmp);

    double prediction = train_Left.value().compute_Predicted_Value();
    train_Left.value().node->set_Predicted_Value(prediction);
  }
  return {train_Left, train_Right};
}

//
void HistogramTrainingElement::train(const DataSet &data, TreeNode *node,
                                     uint16_t max_Depth) {

  HistogramTrainingElement base_Node;

  // Initialize the root Node
  base_Node.set_Root(node);
  base_Node.init_Histograms(data);

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

    auto [left, right] = elem.split_Node(data);

    if (left) {
      remaining.push(std::move(*left));
    }

    if (right) {
      remaining.push(std::move(*right));
    }
  }
}