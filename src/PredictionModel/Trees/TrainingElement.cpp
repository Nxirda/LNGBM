#include "TrainingElement.hpp"

#include <functional>
#include <omp.h>
#include <random>
#include <stack>
#include <thread>
#include <tuple>

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
TrainingElement::TrainingElement(TreeNode *node, const std::vector<size_t> &index,
                                 uint16_t depth) {
  this->node = node;
  this->depth = depth;
  this->index = std::move(index);
}

//
TrainingElement::TrainingElement(const TrainingElement &TE) {
  this->depth = TE.depth;
  this->index = TE.index;
  this->node = TE.node;
}

//
TrainingElement &TrainingElement::operator=(const TrainingElement &TE) {
  this->depth = TE.depth;
  this->index = TE.index;
  this->node = TE.node;
  return *this;
}

//
TrainingElement::~TrainingElement(){};

//
const std::vector<size_t> &TrainingElement::get_Index() { return this->index; }

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
  this->set_Index(std::move(idx));
}

//
std::tuple<size_t, double>
TrainingElement::find_Best_Split(const DataSet &data,
                                 const IOperator *splitting_Operator,
                                 const ICriteria *splitting_Criteria) {

  size_t best_Feature = 0;
  double best_Criterion = 0.0;
  // We try to minimize the mean absolute error for a split
  double min = INT_MAX;

  // Minimize the error by trying multiple splits on features
  for (size_t i = 0; i < data.get_Features().size(); ++i) {

    std::vector<double> criterias =
        splitting_Criteria->compute(data.get_Column(i), this->index);

    // Test multiple split criteria
    for (size_t j = 0; j < criterias.size(); ++j) {

      double tmp_var =
          splitting_Operator->compute(i, data, this->index, criterias[j]);

      if (tmp_var < min) {
        min = tmp_var;
        best_Feature = i;
        best_Criterion = criterias[j];
      }
    }
  }
  return {best_Feature, best_Criterion};
}

//
std::tuple<std::optional<std::vector<size_t>>, std::optional<std::vector<size_t>>>
TrainingElement::split_Index(const DataSet &data, double criterion, size_t position) {

  return data.split(position, criterion, this->get_Index());
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
  auto [column, criterion] = this->find_Best_Split_Parallel(
      data, splitting_Operator, splitting_Criteria);
  // this->find_Best_Split(data, splitting_Operator, splitting_Criteria);

  // Compute new indexes
  auto [left_index, right_index] = split_Index(data, criterion, column);

  if (!left_index && !right_index) {
    return {train_Left, train_Right};
  }

  uint16_t next_Depth = this->depth + 1;

  // Set the datas for the current node
  this->node->set_Split_Column(column);
  this->node->set_Split_Criterion(criterion);

  // Case 1 : Build Right Node (if information gained)
  if (right_index.has_value()) {
    double predic_Right = data.labels_Mean(*right_index);
    this->node->add_Right(std::make_unique<TreeNode>(std::move(right)));
    this->node->get_Right_Node()->set_Predicted_Value(predic_Right);
    train_Right = TrainingElement(this->node->get_Right_Node(),
                                  std::move(*right_index), next_Depth);
  }

  // Case 2 : Build Left Node (if information gained)
  if (left_index.has_value()) {
    double predic_Left = data.labels_Mean(*left_index);
    this->node->add_Left(std::make_unique<TreeNode>(std::move(left)));
    this->node->get_Left_Node()->set_Predicted_Value(predic_Left);
    train_Left = TrainingElement(this->node->get_Left_Node(),
                                 std::move(*left_index), next_Depth);
  }

  return {train_Left, train_Right};
}

//
void TrainingElement::train(const DataSet &data, TreeNode *Node,
                            const IOperator *splitting_Operator,
                            const ICriteria *splitting_Criteria, uint16_t max_Depth,
                            size_t threshold) {

  // Initialize the stack of Node that will be splitted
  std::stack<TrainingElement> remaining;

  TrainingElement base_Node{};

  // Initialize the current Node
  base_Node.set_Root(data.labels_Number(), Node);
  base_Node.node->set_Predicted_Value(data.labels_Mean(base_Node.index));

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

/*******************************************************************/

// NEED CLEANUP

// Maybe do two versions :
//  -> one if num_Features < nb Threads (in this case focus more on the second
//  loop)
//  -> one if num_Features > nb_Threads (in this case focus more on the first
//  loop)
// Of course compare it to the second loop range (to use full "thread power")

// Outter loop parallelism
auto thread_Task =
    [](const IOperator *splitting_Operator, const ICriteria *splitting_Criteria,
       const std::vector<size_t> &index,
       std::vector<std::tuple<size_t, double, double>> &res,
       const DataSet &data, size_t start, size_t end, size_t tid) {
      // We try to minimize the result for a split
      double local_min = INT_MAX;
      size_t best_Feature = 0;
      double best_Criterion = 0.0;

      for (size_t i = start; i < end; ++i) {
        std::vector<double> criterias =
            splitting_Criteria->compute(data.get_Column(i), index);

        // Test multiple split criteria
        for (size_t j = 0; j < criterias.size(); ++j) {

          double tmp_var =
              splitting_Operator->compute(i, data, index, criterias[j]);

          if (tmp_var < local_min) {
            local_min = tmp_var;
            best_Feature = i;
            best_Criterion = criterias[j];
          }
        }
      }
      res[tid] = {best_Feature, best_Criterion, local_min};
    };

//
std::tuple<size_t, double>
TrainingElement::find_Best_Split_Parallel(const DataSet &data,
                                          const IOperator *splitting_Operator,
                                          const ICriteria *splitting_Criteria) {

  size_t best_Feature = 0;
  double criterion = 0.0;

  // We try to minimize the mean absolute error for a split
  double min = INT_MAX;
  size_t size = data.get_Features().size();

  //
  size_t num_Threads =
      std::min(size, (size_t)std::thread::hardware_concurrency());

  std::vector<std::thread> threads(num_Threads);
  std::vector<std::tuple<size_t, double, double>> res(num_Threads);

  size_t chunk_Size = size / num_Threads;

  for (size_t tid = 0; tid < num_Threads; ++tid) {

    // Chunks computing
    size_t start = tid * chunk_Size;
    size_t end = std::min(start + chunk_Size, size);

    threads[tid] = std::thread(thread_Task, splitting_Operator,
                               splitting_Criteria, std::cref(this->index),
                               std::ref(res), std::cref(data), start, end, tid);
  }

  for (size_t tid = 0; tid < num_Threads; ++tid) {
    threads[tid].join();

    if (std::get<2>(res[tid]) < min) {
      best_Feature = std::get<0>(res[tid]);
      criterion = std::get<1>(res[tid]);
      min = std::get<2>(res[tid]);
    }
  }

  return {best_Feature, criterion};
}