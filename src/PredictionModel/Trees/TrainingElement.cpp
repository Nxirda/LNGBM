#include "TrainingElement.hpp"

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

/*
Default Constructor
Parameters :
Inputs     :
Outputs    : Object of TrainingElement class
*/
TrainingElement::TrainingElement() {
  this->depth = 0;
  this->node = nullptr;
  this->index = std::vector<int>();
}

/*
Constructor to set up the node of the training element
Parameters : Node, index, depth
Inputs     : TreeNode*, const vector<int>, int
Outputs    : Object of TrainingElement class
*/
TrainingElement::TrainingElement(TreeNode *node, const std::vector<int> &index,
                                 int depth) {
  this->node = node;
  this->depth = depth;
  this->index = std::move(index);
}

/*
Copy Assignment
Parameters : Training Element to copy
Inputs     : const TrainingElement
Outputs    : Object of TrainingElement class
 */
TrainingElement::TrainingElement(const TrainingElement &TE) {
  this->depth = TE.depth;
  this->index = TE.index;
  this->node = TE.node;
}

/*
Copy constructor
Parameters : Training Element to copy
Inputs     : const TrainingElement
Outputs    : Object of TrainingElement class
 */
TrainingElement &TrainingElement::operator=(const TrainingElement &TE) {
  this->depth = TE.depth;
  this->index = TE.index;
  this->node = TE.node;
  return *this;
}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
TrainingElement::~TrainingElement(){};

/*
Returns the index the TrainingElement can access
Parameters :
Inputs     :
Outputs    : vector<int>
*/
const std::vector<int> &TrainingElement::get_Index() { return this->index; }

/*
Sets the node pointer to a given node
Parameters : node
Inputs     : TreeNode*
Outputs    :
*/
void TrainingElement::set_Node(TreeNode *node) { this->node = node; }

/*
Sets the index
Parameters : index
Inputs     : vector<int>
Outputs    :
*/
void TrainingElement::set_Index(const std::vector<int> &index) {
  this->index = index;
}

/*
Sets the depth
Parameters : depth
Inputs     : int
Outputs    :
*/
void TrainingElement::set_depth(int depth) { this->depth = depth; }

/*
Sets the root element (First node of the tree)
Parameters : DataSet size, TreeNode, predicted value
Inputs     : int, TreeNode*, double
Outputs    :
*/
void TrainingElement::set_Root(int dataset_Size, TreeNode *node) {
  this->depth = 0;

  bootstrap_Index(dataset_Size);
  this->node = node;
}

/*
Bootstraps the index of a node by sort of shuffling it
Parameters : DataSet size
Inputs     : int
Outputs    :
*/
void TrainingElement::bootstrap_Index(int dataset_Size) {

  // Generate a unique seed using hardware entropy

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, dataset_Size - 1);

  std::vector<int> idx(dataset_Size);
  for (int i = 0; i < dataset_Size; ++i) {
    idx[i] = dist(gen);
  }
  this->set_Index(std::move(idx));
}

/*
Search for the best feature to split the dataset on at a given Node
Gives the split criterion at the same time
Parameters : Dataset, Element, Splitting operator, Splitting Criteria
Inputs     : const DataSet, TrainingElem*, IOperator*, ICriteria*
Ouputs     : tuple<int, double>
*/
std::tuple<int, double>
TrainingElement::find_Best_Split(const DataSet &data,
                                 const IOperator *splitting_Operator,
                                 const ICriteria *splitting_Criteria) {

  int best_Feature = 0;
  double criterion = 0.0;
  // We try to minimize the mean absolute error for a split
  double min = INT_MAX;

  // Minimize the error by trying multiple splits on features
  //#pragma omp parallel for
  for (size_t i = 0; i < data.get_Features().size(); ++i) {

    std::vector<double> criterias =
        splitting_Criteria->compute(data.get_Column(i), this->index);

    int local_Best_Feature = 0;
    double local_Criterion = 0.0;

    // We try to minimize the result for a split
    double local_min = INT_MAX;

    // Test multiple split criteria
    for (size_t j = 0; j < criterias.size(); ++j) {

      double tmp_var =
          splitting_Operator->compute(i, data, this->index, criterias[j]);

      // so the threads are "independant", they'll wait each other for
      // comparison each iteration
      if (tmp_var < local_min) {
        local_min = tmp_var;
        local_Best_Feature = i;
        local_Criterion = criterias[j];
      }
    }
    //#pragma omp critical
    //{
    if (local_min < min) {
      min = local_min;
      best_Feature = local_Best_Feature;
      criterion = local_Criterion;
    }
    //}
  }
  return {best_Feature, criterion};
}

/*
Returns two vectors containing the indexes of each sub dataset after splitting
Parameters : Dataset, criterion, position, element
Inputs     : const DataSet, int, int, TrainingElement*
Outputs    : tuple<optional<vector<int>>, optional<vector<int>>>
*/
std::tuple<std::optional<std::vector<int>>, std::optional<std::vector<int>>>
TrainingElement::split_Index(const DataSet &data, int criterion, int position) {

  return data.split(position, criterion, this->get_Index());
}

/*
Splits the element according to the specified operator
Parameters : Dataset, element, operator, criteria
Inputs     : const DataSet, TrainingElement*, IOperator*, ICriteria*
Outputs    : tuple<optional<TrainingElement>, <optional<TrainingElement>>
*/
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
  auto [left_index, right_index] =
      split_Index(data, criterion, column );

  if (!left_index && !right_index) {
    return {train_Left, train_Right};
  }

  int next_Depth = this->depth + 1;

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

/*
Train the model by splitting nodes till they have reach max Depth or dont
bring anymore informations Parameters : Dataset, operator, criteria, max
depth, treshold Inputs     : const DataSet, IOperator*, ICriteria*, int, long
unsigned int Outputs    :
*/
void TrainingElement::train(const DataSet &data, TreeNode *Node,
                            const IOperator *splitting_Operator,
                            const ICriteria *splitting_Criteria, int max_Depth,
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
//  -> one if num_Features < nb Threads (in this case focus more on the second loop)
//  -> one if num_Features > nb_Threads (in this case focus more on the first  loop)
//Of course compare it to the second loop range (to use full "thread power")

//
std::tuple<int, double>
TrainingElement::find_Best_Split_Parallel(const DataSet &data,
                                          const IOperator *splitting_Operator,
                                          const ICriteria *splitting_Criteria) {

  int best_Feature = 0;
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

    threads[tid] = std::thread([this, &res, &data, splitting_Operator,
                                splitting_Criteria, start, end, tid]() {
      // We try to minimize the result for a split
      double local_min = INT_MAX;
      size_t best_Feature = 0;
      double best_Criterion = 0.0;

      for (size_t i = start; i < end; ++i) {
        std::vector<double> criterias =
            splitting_Criteria->compute(data.get_Column(i), this->index);

        // Test multiple split criteria
        for (size_t j = 0; j < criterias.size(); ++j) {

          double tmp_var =
              splitting_Operator->compute(i, data, this->index, criterias[j]);

          if (tmp_var < local_min) {
            local_min = tmp_var;
            best_Feature = i;
            best_Criterion = criterias[j];
          }
        }
      }
      res[tid] =
          std::move(std::make_tuple(best_Feature, best_Criterion, local_min));
    });
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