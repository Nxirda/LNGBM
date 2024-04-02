#include <algorithm>
#include <cblas.h>
#include <omp.h>
#include <random>
#include <stack>
#include <tuple>

#include <ranges>

#include "MAE.hpp"
#include "TrainingElement.hpp"

/**************************/
/*                        */
/* TRAINING ELEMENT CLASS */
/*                        */
/**************************/

//
TrainingElement::TrainingElement() {
  this->depth = 0;
  this->node = nullptr;
  this->index = std::vector<size_t>();
}

//
TrainingElement::TrainingElement(TreeNode *node,
                                 const std::vector<size_t> &index,
                                 uint16_t depth) {
  this->node = node;
  this->depth = depth;
  this->index = std::move(index);
}

//
TrainingElement::TrainingElement(const TrainingElement &TE) {
  this->node = TE.node;
  this->depth = TE.depth;
  this->index = TE.index;
}

//
TrainingElement &TrainingElement::operator=(const TrainingElement &TE) {
  this->node = TE.node;
  this->depth = TE.depth;
  this->index = TE.index;
  return *this;
}

//
TrainingElement::~TrainingElement(){};

//
const std::vector<size_t> &TrainingElement::get_Index() const {
  return this->index;
}

//
void TrainingElement::set_Node(TreeNode *node) { this->node = node; }

//
void TrainingElement::set_Index(const std::vector<size_t> &index) {
  this->index = index;
}

//
void TrainingElement::set_depth(uint16_t depth) { this->depth = depth; }

//
void TrainingElement::set_Root(size_t dataset_Size, TreeNode *node) {
  this->depth = 0;
  bootstrap_Index(dataset_Size);
  this->node = node;
}

//
void TrainingElement::bootstrap_Index(size_t dataset_Size) {

  // Generate a unique seed using hardware entropy
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> dist(0, dataset_Size - 1);
  std::vector<size_t> idx(dataset_Size);

  for (size_t i = 0; i < dataset_Size; ++i) {
    idx[i] = dist(gen);
  }
  this->index = std::move(idx);
}

//
double
TrainingElement::mean_Vector_At_Index(const std::vector<double> &vector,
                                      const std::vector<size_t> &idx) const {
  if (idx.empty()) {
    return 0.0;
  }

  double mean = 0.0;
  const double len = idx.size();

  for (const auto &i : idx) {
    mean += vector[i];
  }

  mean *= (1.0 / len);
  return (mean);
}

//
double TrainingElement::compute_Split_Value(const std::vector<size_t> &index,
                                            const DataSet &data, size_t feature,
                                            double criteria,
                                            const IOperator *op) const {

  auto [left_Labels, right_Labels] = split_Labels(
      data.get_Column(feature), data.get_Labels(), criteria, index);

  if (!left_Labels && !right_Labels) {
    return -1.0;
  }

  const size_t base_Population = index.size();
  const size_t left_Population = left_Labels.value().size();
  const size_t right_Population = right_Labels.value().size();

  double left_Metric_Result = 0.0;
  double right_Metric_Result = 0.0;

  double left_Prediction =
      cblas_dasum(left_Population, left_Labels.value().data(), 1.0) * 1.0 /
      left_Population;

  double right_Prediction =
      cblas_dasum(right_Population, right_Labels.value().data(), 1.0) * 1.0 /
      right_Population;

  left_Metric_Result = op->compute(left_Labels.value(), left_Prediction);
  right_Metric_Result = op->compute(right_Labels.value(), right_Prediction);

  // Compute the result of the metric for the split at position
  double res = ((left_Metric_Result * left_Population) +
                (right_Metric_Result * right_Population));

  res *= (1.0 / base_Population);

  return res;
}

//
std::tuple<std::optional<std::vector<size_t>>,
           std::optional<std::vector<size_t>>>
TrainingElement::split_Index(const std::vector<double> &column,
                             const std::vector<size_t> &index,
                             double criterion) {

  // Check column in bounds
  if (index.empty()) {
    return {std::nullopt, std::nullopt};
  }

  std::vector<size_t> sub_Index_Right;
  std::vector<size_t> sub_Index_Left;

  sub_Index_Right.reserve(index.size());
  sub_Index_Left.reserve(index.size());

  for (const auto &row : index) {
    if (column[row] < criterion) {
      sub_Index_Left.push_back(row);
    } else {
      sub_Index_Right.push_back(row);
    }
  }

  return std::make_pair(std::move(sub_Index_Left), std::move(sub_Index_Right));
}

//
std::tuple<std::optional<std::vector<size_t>>,
           std::optional<std::vector<size_t>>>
TrainingElement::split_Index_Views(const std::vector<double> &column,
                                   const std::vector<size_t> &index,
                                   double criterion) const {
  // Check if index is empty
  if (index.empty()) {
    return {std::nullopt, std::nullopt};
  }

  // Create a filtered view for the indices based on the criterion
  auto left_Indices_View = index | std::views::filter([&](size_t i) {
                             return column[i] < criterion;
                           });
  auto right_Indices_View = index | std::views::filter([&](size_t i) {
                              return column[i] >= criterion;
                            });

  std::vector<size_t> left(std::ranges::begin(left_Indices_View),
                           std::ranges::end(left_Indices_View));
  std::vector<size_t> right(std::ranges::begin(right_Indices_View),
                            std::ranges::end(right_Indices_View));

  // Return the views directly
  return std::make_pair(std::move(left), std::move(right));
}

//
std::tuple<std::optional<std::vector<double>>,
           std::optional<std::vector<double>>>
TrainingElement::split_Labels(const std::vector<double> &column,
                              const std::vector<double> &labels,
                              double criterion,
                              const std::vector<size_t> &idx) const {
  // Check column in bounds
  if (idx.empty()) {
    return {std::nullopt, std::nullopt};
  }

  auto [left_Index, right_Index] = split_Index(column, idx, criterion);

  std::vector<double> sub_Labels_Left(left_Index.value().size());
  std::vector<double> sub_Labels_Right(right_Index.value().size());

  /* auto extractor = [&labels](size_t index) -> const double & {
    return labels[index];
  }; */

  /* auto labels_Left_View = left_Index.value() |
  std::views::transform(extractor); auto labels_Right_View = right_Index.value()
  | std::views::transform(extractor); */
  /* std::ranges::transform(left_Index.value(), sub_Labels_Left.begin(),
                         extractor);
  std::ranges::transform(right_Index.value(), sub_Labels_Right.begin(),
                         extractor); */

  size_t i;
  for (i = 0; i < sub_Labels_Left.size(); ++i) {
    const auto &row = left_Index.value()[i];
    sub_Labels_Left[i] = labels[row];
  }

  for (i = 0; i < sub_Labels_Right.size(); ++i) {
    const auto &row = right_Index.value()[i];
    sub_Labels_Right[i] = labels[row];
  }

  return std::make_pair(std::move(sub_Labels_Left),
                        std::move(sub_Labels_Right));
}

//
std::tuple<std::optional<TrainingElement>, std::optional<TrainingElement>>
TrainingElement::split_Node(const DataSet &data,
                            const IOperator *splitting_Operator,
                            const ICriteria *splitting_Criteria) {
  // Left node
  TreeNode left{};
  std::optional<TrainingElement> train_Left = std::nullopt;

  // Right node
  TreeNode right{};
  std::optional<TrainingElement> train_Right = std::nullopt;

  // Compute split attributes
  auto [column, criterion] =
      find_Best_Split(data, splitting_Operator, splitting_Criteria);

  // Compute new indexes
  auto [left_index, right_index] =
      split_Index(data.get_Column(column), this->get_Index(), criterion);

  if (!left_index && !right_index) {
    return {train_Left, train_Right};
  }

  uint16_t next_Depth = this->depth + 1;

  // Set the datas for the current node
  this->node->set_Split_Column(column);
  this->node->set_Split_Criterion(criterion);

  // Case 1 : Build Right Node (if information gained)
  if (right_index.has_value()) {
    double predic_Right = mean_Vector_At_Index(data.get_Labels(), *right_index);
    this->node->add_Right(std::make_unique<TreeNode>(std::move(right)));
    this->node->get_Right_Node()->set_Predicted_Value(predic_Right);
    train_Right = std::move(TrainingElement(
        this->node->get_Right_Node(), std::move(*right_index), next_Depth));
  }

  // Case 2 : Build Left Node (if information gained)
  if (left_index.has_value()) {
    double predic_Left = mean_Vector_At_Index(data.get_Labels(), *left_index);
    this->node->add_Left(std::make_unique<TreeNode>(std::move(left)));
    this->node->get_Left_Node()->set_Predicted_Value(predic_Left);
    train_Left = std::move(TrainingElement(this->node->get_Left_Node(),
                                           std::move(*left_index), next_Depth));
  }

  return {train_Left, train_Right};
}

/******************************* OPEN MP THREADS*******************************/

std::tuple<size_t, double>
TrainingElement::find_Best_Split(const DataSet &data,
                                 const IOperator *splitting_Operator,
                                 const ICriteria *splitting_Criteria) const {
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

#pragma omp for schedule(static)
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
}

//
void TrainingElement::train(const DataSet &data, TreeNode *Node,
                            const IOperator *splitting_Operator,
                            const ICriteria *splitting_Criteria,
                            uint16_t max_Depth, size_t threshold) {
  // Initialize the stack of Node that will be splitted
  std::stack<TrainingElement> remaining;

  TrainingElement base_Node{};

  // Initialize the current Node
  base_Node.set_Root(data.labels_Number(), Node);
  double base_Prediction =
      base_Node.mean_Vector_At_Index(data.get_Labels(), base_Node.index);
  base_Node.node->set_Predicted_Value(base_Prediction);
  remaining.push(base_Node);

  // Build iteratively the tree frame
  while (not remaining.empty()) {
    auto elem = remaining.top();

    remaining.pop();

    if (elem.depth >= max_Depth) {
      continue;
    }

    auto [left, right] =
        elem.split_Node(data, splitting_Operator, splitting_Criteria);

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

/******************************* HAND WRITTEN THREADS **********************/
// Outter loop parallelism
/* auto thread_Task = [](const IOperator *splitting_Operator,
                      const ICriteria *splitting_Criteria,
                      const std::vector<size_t> &index, const DataSet &data,
                      size_t start, size_t end, size_t tid,
                      std::vector<std::tuple<size_t, double, double>> &res) {
  // We try to minimize the result for a split
  double local_min = INT_MAX;
  size_t best_Feature = 0;
  double best_Criterion = 0.0;

  size_t i, j;
  double tmp_var;

  for (i = start; i < end; ++i) {
    std::vector<double> criterias =
        splitting_Criteria->compute(data.get_Column(i), index);

    // Test multiple split criteria
    for (j = 0; j < splitting_Criteria->get_Criteria_Number(); ++j) {

      tmp_var = splitting_Operator->compute(i, data, index, criterias[j]);

      if (tmp_var < local_min) {
        local_min = tmp_var;
        best_Feature = i;
        best_Criterion = criterias[j];
      }
    }
  }
  res[tid] = {best_Feature, best_Criterion, local_min};
}; */

//
/* std::tuple<size_t, double>
TrainingElement::find_Best_Split_Parallel(const DataSet &data,
                                          const IOperator *splitting_Operator,
                                          const ICriteria *splitting_Criteria)
{

  // We try to minimize the mean absolute error for a split
  double min = INT_MAX;
  double criterion = 0.0;
  size_t best_Feature = 0;
  size_t size = data.get_Features().size();

  uint16_t num_Threads =
      std::min<uint16_t>(size, (size_t)std::thread::hardware_concurrency());

  // Thread pool
  std::vector<std::thread> threads(num_Threads);
  std::vector<std::tuple<size_t, double, double>> res(num_Threads);

  // For the threads
  size_t end;
  size_t start;
  size_t chunk_Size = size / num_Threads;

  uint16_t tid;
  // Thread creation && task set
  for (tid = 0; tid < num_Threads; ++tid) {

    // Chunks computing
    start = tid * chunk_Size;
    end = std::min(start + chunk_Size, size);

    // Maybe ensure thread creation here
    threads[tid] = std::thread(thread_Task, splitting_Operator,
                               splitting_Criteria, std::cref(this->index),
                               std::cref(data), start, end, tid,
std::ref(res));
  }

  // Thread join && result aggregation
  for (tid = 0; tid < num_Threads; ++tid) {
    threads[tid].join();

    auto &result = res[tid];
    if (std::get<2>(result) < min) {
      best_Feature = std::get<0>(result);
      criterion = std::get<1>(result);
      min = std::get<2>(result);
    }
  }

  return {best_Feature, criterion};
} */

/******************************* INITIAL FUNCTION **********************/

//
/* std::tuple<size_t, double>
TrainingElement::find_Best_Split(const DataSet &data,
                                 const IOperator *splitting_Operator,
                                 const ICriteria *splitting_Criteria) {

  // We try to minimize the mean absolute error for a split
  double min = INT_MAX;
  size_t best_Feature = 0;
  double best_Criterion = 0.0;

  size_t feature, j;
  double tmp_var;
  // Minimize the error by trying multiple splits on features
  for (feature = 0; feature < data.get_Features().size(); ++feature) {

    std::vector<double> criterias =
        splitting_Criteria->compute(data.get_Column(feature), this->index);

    // Test multiple split criteria
    for (j = 0; j < criterias.size(); ++j) {

      tmp_var =
          splitting_Operator->compute(feature, data, this->index,
criterias[j]);

      splitting_Operator->get_Name();
      tmp_var = compute_Split_Value(this->index, data, feature, criterias[j],
                                    splitting_Operator);

      if (tmp_var < min) {
        min = tmp_var;
        best_Feature = feature;
        best_Criterion = criterias[j];
      }
    }
  }
  return {best_Feature, best_Criterion};
} */

//
/* void find_Best_Split_Test(
    const DataSet &data, const TrainingElement &node,
    const IOperator *splitting_Operator, const ICriteria *splitting_Criteria,
    std::vector<std::tuple<double, double, size_t>> &candidates) {

  std::vector<std::vector<double>> splitting_Thresholds;
  splitting_Thresholds.resize(data.features_Number());

  size_t column;
#pragma omp parallel
  {
    int thread_Id = omp_get_thread_num();
#pragma omp for schedule(static)
    for (column = 0; column < data.features_Number(); ++column) {
      splitting_Thresholds[column] = splitting_Criteria->compute(
          data.get_Column(column), node.get_Index());
    }
#pragma omp single
    {
      candidates.resize(omp_get_num_threads(),
                        {std::numeric_limits<double>::max(), 0.0, 0});
    }
#pragma omp for schedule(static)
    for (column = 0; column < data.features_Number(); ++column) {
      double split_Score;
      for (const auto splitting_Threshold : splitting_Thresholds[column]) {
        split_Score =
            node.compute_Split_Value(node.get_Index(), data, column,
                                     splitting_Threshold, splitting_Operator);
        if (split_Score < std::get<0>(candidates[thread_Id])) {

          candidates[thread_Id] = {split_Score, splitting_Threshold, column};
        }
      }
    }
  }
} */

/************************** STATIC TEST PART***********************************/

/* void TrainingElement::mean_Vector_At_Index_Test(
    const std::vector<double> &vector, const std::vector<size_t> &idx,
    double &mean, double &length) {

#pragma omp single
  {
    mean = 0.0;
    length = 0.0;
  }

  if (!idx.empty()) { // Necessary because we cant exit properly a function in a
                      // parallel region
    size_t size = vector.size();

#pragma omp for reduction(+ : length) reduction(+ : mean) schedule(static)
    for (const auto &i : idx) {
      if (i < size) {
        length += 1.0;
        mean += vector[i];
      } else {
        std::cerr << "Index points outside the vector\n";
      }
    }

#pragma omp single
    {
      // To prevent dividing by 0
      if (length == 0.0) {
        mean = 0.0;
        std::cerr << "Index is empty/invalid in the mean computation\n";
      } else
        mean *= (1.0 / length);
    }
  }
}

//
void TrainingElement::split_Index_Test(
    const std::vector<double> &column, const std::vector<size_t> &index,
    double criterion, std::vector<size_t> &right_Index,
    std::vector<size_t> &left_Index,
    std::vector<std::vector<size_t>> &right_Index_Per_Threads,
    std::vector<std::vector<size_t>> &left_Index_Per_Threads) {

  size_t size = column.size();
  int tid = omp_get_thread_num();

  std::vector<size_t> sub_Index_Right;
  std::vector<size_t> sub_Index_Left;

  sub_Index_Right.reserve(index.size());
  sub_Index_Left.reserve(index.size());

#pragma omp for
  for (const auto &row : index) {
    //  Row in bounds (matrix is transposed)
    if (row >= size) {
      std::cerr << "row index is out of the matrix\n";
    } else {
      if (column[row] < criterion) {
        sub_Index_Left.push_back(row);
      } else {
        sub_Index_Right.push_back(row);
      }
    }
  }
  right_Index_Per_Threads[tid] = std::move(sub_Index_Right);
  left_Index_Per_Threads[tid] = std::move(sub_Index_Left);

#pragma omp barrier

#pragma omp single
  {
#pragma omp task
    {
      for (int i = 0; i < right_Index_Per_Threads.size(); ++i) {
        right_Index.insert(right_Index.end(),
                           right_Index_Per_Threads[i].begin(),
                           right_Index_Per_Threads[i].end());
      }
    }

#pragma omp task
    {
      for (int j = 0; j < left_Index_Per_Threads.size(); ++j) {
        left_Index.insert(left_Index.end(), left_Index_Per_Threads[j].begin(),
                          left_Index_Per_Threads[j].end());
      }
    }
  }
}

//
std::tuple<std::optional<std::vector<double>>,
           std::optional<std::vector<double>>>
TrainingElement::split_Labels_Test(const std::vector<double> &column,
                                   const std::vector<double> &labels,
                                   double criterion,
                                   const std::vector<size_t> &idx) {
  // Check column in bounds
  if (idx.empty()) {
    return {std::nullopt, std::nullopt};
  }

  size_t size = column.size();

  std::vector<double> sub_Labels_Right;
  std::vector<double> sub_Labels_Left;

  sub_Labels_Right.reserve(idx.size());
  sub_Labels_Left.reserve(idx.size());

  for (const auto &row : idx) {
    //  Row in bounds (matrix is transposed)
    if (row >= size) {
      std::cerr << "row index is out of the matrix\n";
    } else {
      if (column[row] < criterion) {
        sub_Labels_Left.push_back(labels[row]);
      } else {
        sub_Labels_Right.push_back(labels[row]);
      }
    }
  }

  return std::make_pair(std::move(sub_Labels_Left),
                        std::move(sub_Labels_Right));
}

//
double TrainingElement::compute_Split_Value_Test(
    const std::vector<size_t> &index, const DataSet &data, size_t feature,
    double criteria, const IOperator *op) {

  auto [left_Labels, right_Labels] = split_Labels_Test(
      data.get_Column(feature), data.get_Labels(), criteria, index);

  if (!left_Labels && !right_Labels) {
    return -1.0;
  }

  size_t base_Population = index.size();

  double left_Metric_Result = 0.0;
  size_t left_Population = left_Labels.value().size();

  double left_Prediction =
      cblas_dasum(left_Population, left_Labels.value().data(), 1.0) *
      (1.0 / left_Population);

  double right_Metric_Result = 0.0;
  size_t right_Population = right_Labels.value().size();

  double right_Prediction =
      cblas_dasum(right_Population, right_Labels.value().data(), 1.0) *
      (1.0 / right_Population);

  left_Metric_Result = op->compute(left_Labels.value(), left_Prediction);
  right_Metric_Result = op->compute(right_Labels.value(), right_Prediction);

  // Compute the result of the metric for the split at position
  double res = ((left_Metric_Result * left_Population) +
                (right_Metric_Result * right_Population));

  res *= (1.0 / base_Population);

  return res;
}

//
void TrainingElement::find_Best_Split_Test(
    const DataSet &data, const std::vector<size_t> &index,
    const IOperator *splitting_Operator, const ICriteria *splitting_Criteria,
    std::vector<std::tuple<double, double, size_t>> &candidates,
    std::vector<std::vector<double>> &splitting_Thresholds) {

  size_t column;
  int thread_Id = omp_get_thread_num();

#pragma omp for schedule(static)
  for (column = 0; column < data.features_Number(); ++column) {
    splitting_Thresholds[column] =
        splitting_Criteria->compute(data.get_Column(column), index);
  }

#pragma omp for schedule(static)
  for (column = 0; column < data.features_Number(); ++column) {
    double split_Score;
    for (const auto splitting_Threshold : splitting_Thresholds[column]) {
      split_Score = compute_Split_Value_Test(
          index, data, column, splitting_Threshold, splitting_Operator);

      if (split_Score < std::get<0>(candidates[thread_Id])) {
        candidates[thread_Id] = {split_Score, splitting_Threshold, column};
      }
    }
  }
}

//
std::tuple<std::optional<TrainingElement>, std::optional<TrainingElement>>
TrainingElement::split_Node_Test(const DataSet &data, size_t column,
                                 double criterion, double predic_Left,
                                 double predic_Right,
                                 std::vector<size_t> &right_Index,
                                 std::vector<size_t> &left_Index) {
  // Left node
  std::optional<TrainingElement> train_Left = std::nullopt;

  // Right node
  std::optional<TrainingElement> train_Right = std::nullopt;

  uint16_t next_Depth = this->depth + 1;

  // Set the datas for the current node
  this->node->set_Split_Column(column);
  this->node->set_Split_Criterion(criterion);

  // Case 1 : Build Right Node
  if (!right_Index.empty()) {
    TreeNode right{};
    this->node->add_Right(std::make_unique<TreeNode>(std::move(right)));
    this->node->get_Right_Node()->set_Predicted_Value(predic_Right);
    train_Right = std::move(TrainingElement(
        this->node->get_Right_Node(), std::move(right_Index), next_Depth));
  }

  // Case 2 : Build Left Node
  if (!left_Index.empty()) {
    TreeNode left{};
    this->node->add_Left(std::make_unique<TreeNode>(std::move(left)));
    this->node->get_Left_Node()->set_Predicted_Value(predic_Left);
    train_Left = std::move(TrainingElement(this->node->get_Left_Node(),
                                           std::move(left_Index), next_Depth));
  }

  return {train_Left, train_Right};
}

//
void TrainingElement::train_Test(const DataSet &data, TreeNode *Node,
                                 const IOperator *splitting_Operator,
                                 const ICriteria *splitting_Criteria,
                                 uint16_t max_Depth, size_t threshold) {
  // Initialize the stack of Node that will be splitted
  std::stack<TrainingElement> remaining;

  TrainingElement base_Node{};

  // Initialize the current Node
  base_Node.set_Root(data.labels_Number(), Node);
  double base_Prediction =
      base_Node.mean_Vector_At_Index(data.get_Labels(), base_Node.index);
  base_Node.node->set_Predicted_Value(base_Prediction);
  remaining.push(base_Node);

  // Threads shared variables
  std::vector<std::vector<double>> splitting_Thresholds(data.features_Number());
  std::vector<std::tuple<double, double, size_t>> candidates;

  std::vector<size_t> right_Node_Index;
  std::vector<size_t> left_Node_Index;
  std::vector<std::vector<size_t>> right_Index_Per_Thread(8);
  std::vector<std::vector<size_t>> left_Index_Per_Thread(8);

  size_t column;
  double criterion;
  double predic_Left;
  double length_Left;
  double predic_Right;
  double length_Right;

#pragma omp parallel
  {
    while (not remaining.empty()) {
      auto current_Element = remaining.top();

#pragma omp single
      {
#pragma omp task
        { right_Node_Index.clear(); }
#pragma omp task
        { left_Node_Index.clear(); }
#pragma omp task
        {
          candidates.clear();
          candidates.resize(omp_get_num_threads(),
                            {std::numeric_limits<double>::max(), 0.0, 0});
        }
      }

      find_Best_Split_Test(data, current_Element.get_Index(),
                           splitting_Operator, splitting_Criteria, candidates,
                           splitting_Thresholds);
#pragma omp single
      {
        remaining.pop();

        std::tuple<double, double, size_t> best_Split = {-1.0, -1.0, 0};
        for (const auto &candidate : candidates) {
          const auto &best = std::get<0>(best_Split);
          if (best > std::get<0>(candidate) || best == -1.0) {
            best_Split = candidate;
          }
        }

        column = std::get<2>(best_Split);
        criterion = std::get<1>(best_Split);
      } // End of pragma omp single

      split_Index_Test(data.get_Column(column), current_Element.get_Index(),
                       criterion, right_Node_Index, left_Node_Index,
                       right_Index_Per_Thread, left_Index_Per_Thread);

      mean_Vector_At_Index_Test(data.get_Labels(), right_Node_Index,
                                predic_Right, length_Right);

      mean_Vector_At_Index_Test(data.get_Labels(), left_Node_Index, predic_Left,
                                length_Left);

#pragma omp single
      {
        auto [left, right] = current_Element.split_Node_Test(
            data, column, criterion, predic_Left, predic_Right,
            right_Node_Index, left_Node_Index);

        if (left) {
          // Verify we gained information
          if (left.value().index.size() != current_Element.index.size() &&
                  left.value().index.size() > threshold,
              left.value().depth <= max_Depth) {
            remaining.push(std::move(*left));
          }
        }

        if (right) {
          // Verify we gained information
          if (right.value().index.size() != current_Element.index.size() &&
                  right.value().index.size() > threshold,
              right.value().depth <= max_Depth) {
            remaining.push(std::move(*right));
          }
        }
      } // End of pragma omp single
    }
  } // End fo pragma omp parallel
} */