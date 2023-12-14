#include "TrainingElement.hpp"
#include <stack>

/*
Default Constructor
Inputs  :
Outputs :
*/
TrainingElement::TrainingElement() {
  this->depth = 0;
  this->index = std::vector<int>();
  this->node = nullptr;
}

/**/
TrainingElement::TrainingElement(TreeNode *node, std::vector<int> index,
                                 int depth) {
  this->node = node;
  this->index = index;
  this->depth = depth;
}

/**/
TrainingElement::~TrainingElement(){};

/**/
std::vector<int> TrainingElement::get_Index() { return this->index; }

/**/
void TrainingElement::set_Node(TreeNode *node) { this->node = node; }

/**/
void TrainingElement::set_Index(std::vector<int> index) { this->index = index; }

/**/
void TrainingElement::set_depth(int depth) { this->depth = depth; }

/**/
void TrainingElement::set_Root(int dataset_Size, TreeNode *node) {
  this->depth = 0;

  // Computes the index for the given DataSet (Bootstrap directly here)
  std::vector<int> idx(dataset_Size);
  for (int i = 0; i < dataset_Size; ++i) {
    idx[i] = i;
  }

  this->set_Index(idx);
  this->node = node;

  // elem->node->set_Split_Column(column);
  // elem->node->set_Split_Criterion(criterion);
  // elem->node->set_Predicted_Value(data.labels_Mean(elem->get_Index()));
}

/**/
void TrainingElement::bootstrap_Index(int dataset_Size) {
  std::vector<int> idx(dataset_Size);
  for (int i = 0; i < dataset_Size; ++i) {
    idx[i] = rand() % (dataset_Size - 1);
  }
  this->set_Index(idx);
}

/*
Search for the best feature to split the dataset on at a given Node
Gives the split criterion at the same time
Inputs : Dataset, TrainingElem, IOperator
Ouputs : tuple<int, float>
*/
std::tuple<int, float>
TrainingElement::find_Best_Split(const DataSet &data, TrainingElement *elem,
                                 const IOperator *splitting_Operator) {

  /* return splitting_Operator->find_Best_Split(data, elem->index); */
  int best_Feature = 0;
  // We try to minimize the mean absolute error for a split
  float min = INT_MAX;

  std::vector<std::string> features = data.get_Features();

  for (unsigned long int i = 0; i < features.size(); ++i) {
    float tmp_var = splitting_Operator->compute(i, data, elem->index);
    if (tmp_var < min) {
      min = tmp_var;
      best_Feature = i;
    }
  }
  float criterion = data.column_Mean(best_Feature, elem->index);
  return std::make_tuple(best_Feature, criterion);
}

/**/
std::tuple<std::optional<std::vector<int>>, std::optional<std::vector<int>>>
TrainingElement::split_Index(const DataSet &data, int criterion, int position,
                             TrainingElement *elem) {

  if (position < data.features_Length() && criterion > 0) {
    return data.split(position, criterion, elem->get_Index());
  } else {
    return data.split(0, 0, elem->get_Index());
  }
}

/*
Splits the Node according to the specified operator
Inputs  : DataSet, TrainingElement, IOperator*, int
Outputs : tuple<optional<TrainingElement>, <optional<TrainingElement>>
*/
std::tuple<std::optional<TrainingElement>, std::optional<TrainingElement>>
TrainingElement::split_Node(const DataSet &data, TrainingElement *elem,
                            const IOperator *splitting_Operator,
                            int max_Depth) {

  auto [column, criterion] = find_Best_Split(data, elem, splitting_Operator);
  auto [left_index, right_index] = split_Index(data, criterion, column, elem);

  int next_Depth = elem->depth + 1;
  max_Depth++;

  // Set the datas for the current node
  elem->node->set_Split_Column(column);
  elem->node->set_Split_Criterion(criterion);

  //Needs to be fixed, for base node compute it in "set root" maybe
  elem->node->set_Predicted_Value(data.labels_Mean(elem->get_Index()));

  float predic_Left = data.labels_Mean(*left_index);
  float predic_Right = data.labels_Mean(*right_index);

  // SET PRED VAL DIRECTLY HERE
  if (predic_Left > 0 || predic_Right > 0) {
    // Left node
    TreeNode left{};

    // Right node
    TreeNode right{};

    if (predic_Left < 0) {
      elem->node->add_Right(std::make_unique<TreeNode>(right));
      elem->node->get_Right_Node()->set_Predicted_Value(predic_Right);
      TrainingElement train_Right(elem->node->get_Right_Node(), *right_index,
                                  next_Depth);

      return {std::nullopt, train_Right};
    }

    else if (predic_Right < 0) {
      elem->node->add_Left(std::make_unique<TreeNode>(left));
      elem->node->get_Left_Node()->set_Predicted_Value(predic_Left);
      TrainingElement train_Left(elem->node->get_Left_Node(), *left_index,
                                 next_Depth);

      return {train_Left, std::nullopt};

    } else {

      elem->node->add_Left(std::make_unique<TreeNode>(left));
      elem->node->get_Left_Node()->set_Predicted_Value(predic_Left);
      TrainingElement train_Left(elem->node->get_Left_Node(), *left_index,
                                 next_Depth);

      elem->node->add_Right(std::make_unique<TreeNode>(right));
      elem->node->get_Right_Node()->set_Predicted_Value(predic_Right);
      TrainingElement train_Right(elem->node->get_Right_Node(), *right_index,
                                  next_Depth);

      return std::make_tuple(train_Left, train_Right);
    }
  }
  return {std::nullopt, std::nullopt};
}

/**/
void TrainingElement::train(const DataSet &data, IOperator *splitting_Operator,
                            int max_Depth) {
  // Initialize the stack of Node that will be splitted
  std::stack<TrainingElement> remaining;
  //TreeNode root{};

  // Initialize the current Node
  this->set_Root(data.labels_Number(), this->node);
  //this->bootstrap_Index(data.labels_Number());
  remaining.push(*this);

  // Build iteratively the tree frame
  while (not remaining.empty()) {
    auto elem = remaining.top();
    remaining.pop();

    if (elem.depth >= max_Depth) {
      continue;
    }
    auto [left, right] = split_Node(data, &elem, splitting_Operator, max_Depth);
    if (left) {
      remaining.push(*left);
    }
    if (right) {
      remaining.push(*right);
    }
  }
}