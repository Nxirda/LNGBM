#include "TrainingElement.hpp"
#include <omp.h>
#include <stack>

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
void TrainingElement::set_Root(int dataset_Size, TreeNode *node/*, double value*/) {
  this->depth = 0;

  // Computes the index for the given DataSet
  //std::vector<int> idx(dataset_Size);
  //#pragma omp parallel for
  /*for (int i = 0; i < dataset_Size; ++i) {
    idx[i] = i;
  }

  this->set_Index(idx);*/
  bootstrap_Index(dataset_Size);
  this->node = node;
  //this->node->set_Predicted_Value(value);
}

/*
Bootstraps the index of a node by sort of shuffling it
Parameters : DataSet size
Inputs     : int
Outputs    :
*/
void TrainingElement::bootstrap_Index(int dataset_Size) {

  // Generate a unique seed using the current time and the process ID
  //size_t seed =
  //    static_cast<size_t>(std::time(nullptr)) + static_cast<size_t>(getpid());

  // Set the seed for the random number generator
  //std::srand(seed);

  std::vector<int> idx(dataset_Size);
  //#pragma omp parallel for
  for (int i = 0; i < dataset_Size; ++i) {
    idx[i] = rand() % (dataset_Size -1);
  }
  this->set_Index(idx);
  //this->node->set_Predicted_Value()
}

/*
Search for the best feature to split the dataset on at a given Node
Gives the split criterion at the same time
Parameters : Dataset, Element, Splitting operator, Splitting Criteria
Inputs     : const DataSet, TrainingElem*, IOperator*, ICriteria*
Ouputs     : tuple<int, double>
*/
std::tuple<int, double> TrainingElement::find_Best_Split(
    const DataSet &data,
    const IOperator *splitting_Operator, const ICriteria *splitting_Criteria) {

  int best_Feature = 0;
  double criterion = 0;
  // We try to minimize the mean absolute error for a split
  double min = INT_MAX;

  // std::vector<std::string> features = data.get_Features();

  // Minimize the error by trying multiple splits on features
  for (size_t i = 0; i < data.get_Features().size(); ++i) {

    // parallel
    // Index missing
    std::vector<double> criterias =
        splitting_Criteria->compute(data.get_Column(i), this->index);

    // int local_Best_Feature = 0;
    // double local_Criterion = 0;

    // We try to minimize the mean absolute error for a split
    double local_min = INT_MAX;

    // Test multiple split criteria
    for (size_t j = 0; j < criterias.size(); ++j) {

      // Needs to be parallelized
      double tmp_var =
          splitting_Operator->compute(i, data, this->index, criterias[j]);

      if (tmp_var < min) {
        min = tmp_var;
        best_Feature = i;
        criterion = criterias[j];
      }
    }
  }

  // double criterion = data.column_Mean(best_Feature, elem->index);
  return std::make_tuple(best_Feature, criterion);
}

/*
Returns two vectors containing the indexes of each sub dataset after splitting
Parameters : Dataset, criterion, position, element
Inputs     : const DataSet, int, int, TrainingElement*
Outputs    : tuple<optional<vector<int>>, optional<vector<int>>>
*/
std::tuple<std::optional<std::vector<int>>, std::optional<std::vector<int>>>
TrainingElement::split_Index(const DataSet &data, int criterion, int position) {

  if (position < data.features_Length() && criterion > 0) {
    return data.split(position, criterion, this->get_Index());
  } else {
    return {std::nullopt, std::nullopt};
  }
}

/*
Splits the element according to the specified operator
Parameters : Dataset, element, operator, criteria
Inputs     : const DataSet, TrainingElement*, IOperator*, ICriteria*
Outputs    : tuple<optional<TrainingElement>, <optional<TrainingElement>>
*/
std::tuple<std::optional<TrainingElement>, std::optional<TrainingElement>>
TrainingElement::split_Node(const DataSet &data, /*, TrainingElement *elem,*/
                            const IOperator *splitting_Operator,
                            const ICriteria *splitting_Criteria) {

  // Left node
  TreeNode left{};
  std::optional<TrainingElement> train_Left = std::nullopt;

  // Right node
  TreeNode right{};
  std::optional<TrainingElement> train_Right = std::nullopt;

  // Compute split attributes || parallel
  auto [column, criterion] = this->find_Best_Split(
      data, /*elem,*/ splitting_Operator, splitting_Criteria);

  // Compute new indexes || parallel
  auto [left_index, right_index] =
      split_Index(data, criterion, column /* elem*/);

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
    this->node->add_Right(std::make_unique<TreeNode>(right));
    this->node->get_Right_Node()->set_Predicted_Value(predic_Right);
    train_Right =
        TrainingElement(this->node->get_Right_Node(), *right_index, next_Depth);
  }

  // Case 2 : Build Left Node (if information gained)
  if (left_index.has_value()) {
    double predic_Left = data.labels_Mean(*left_index);
    this->node->add_Left(std::make_unique<TreeNode>(left));
    this->node->get_Left_Node()->set_Predicted_Value(predic_Left);
    train_Left =
        TrainingElement(this->node->get_Left_Node(), *left_index, next_Depth);
  }

  return {train_Left, train_Right};
}

/*
Train the model by splitting nodes till they have reach max Depth or dont bring
anymore informations
Parameters : Dataset, operator, criteria, max depth, treshold
Inputs     : const DataSet, IOperator*, ICriteria*, int, long unsigned int
Outputs    :
*/
void TrainingElement::train(const DataSet &data,
                            const IOperator *splitting_Operator,
                            const ICriteria *splitting_Criteria, int max_Depth,
                            size_t treshold) {

  // Initialize the stack of Node that will be splitted
  std::stack<TrainingElement> remaining;

  // Initialize the current Node
  this->set_Root(data.labels_Number(), this->node/*, data.whole_Labels_Mean()*/);
  this->node->set_Predicted_Value(data.labels_Mean(this->index));
  //this->bootstrap_Index(data.labels_Number());
  //this->bootstrap_Index(data.labels_Number());
  //this->set_Root(data.labels_Number(), this->node,
  //              data.labels_Mean(this->index));

  remaining.push(*this);

  // Build iteratively the tree frame
  while (not remaining.empty()) {
    auto elem = remaining.top();
    remaining.pop();

    if (elem.depth >= max_Depth) {
      continue;
    }

    auto [left, right] = elem.split_Node(data, /*&elem,*/ splitting_Operator,
                                         splitting_Criteria);

    if (left) {
      // Verify we gained information
      if (left.value().index.size() != elem.index.size() &&
          left.value().index.size() > treshold) {

        remaining.push(std::move(*left));
      }
    }

    if (right) {
      // Verify we gained information
      if (right.value().index.size() != elem.index.size() &&
          right.value().index.size() > treshold) {

        remaining.push(std::move(*right));
      }
    }
  }
}