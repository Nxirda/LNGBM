#include <fstream>
#include <iostream>
#include <iterator>
#include <stack>
#include <string>
#include <vector>

#include "DecisionTree.hpp"
#include "TrainingElement.hpp"
#include "TreeNode.hpp"

/********************/
/*                  */
/*    TREE CLASS    */
/*                  */
/********************/

/*
Default Constructor
Parameters :
Inputs     :
Outputs    : Object of Decision Tree Class
*/
DecisionTree::DecisionTree() {
  this->root = std::make_unique<TreeNode>();
  this->splitting_Criteria = nullptr;
  this->splitting_Operator = nullptr;
  this->max_Depth = 0;
}

//
DecisionTree::DecisionTree(int max_Depth, ICriteria *crit, IOperator *op) {
  this->root = std::make_unique<TreeNode>();
  this->splitting_Criteria = crit;
  this->splitting_Operator = op;
  this->max_Depth = max_Depth;
}

/*
Constructor to copy a Tree
Parameters : Decision Tree
Inputs     : const DecisionTree
Outputs    : Object of Decision Tree Class
*/
DecisionTree::DecisionTree(const DecisionTree &dt) {
  this->root = std::make_unique<TreeNode>(*dt.get_Root());
  this->splitting_Criteria = dt.splitting_Criteria;
  this->splitting_Operator = dt.splitting_Operator;
}

/**/
DecisionTree &DecisionTree::operator=(const DecisionTree &tree) {
  this->root = std::make_unique<TreeNode>(*tree.get_Root());
  this->splitting_Criteria = tree.splitting_Criteria;
  this->splitting_Operator = tree.splitting_Operator;
  return *this;
}

/*
Default Destructor
Parameters :
Inputs     :
Outputs    :
*/
DecisionTree::~DecisionTree(){};

/*
Returns the Current Node of the Tree
Parameters :
Inputs     :
Outputs    : pointer of Decision Tree Object
*/
TreeNode *DecisionTree::get_Root() const { return this->root.get(); }

/*
Sets the root of the tree (First Node)
Parameters : Tree Node
Inputs     : unique_ptr<TreeNode>
Outputs    :
*/
void DecisionTree::set_Root(std::unique_ptr<TreeNode> node) {
  this->root = std::move(node);
}

/*
Prints the tree
Parameters :
Inputs     :
Outputs    :
*/
void DecisionTree::print_Tree() { this->root->node_Print(); }

//
void DecisionTree::train(const DataSet &data) {
  int treshold = 5;
  TrainingElement elem{};

  elem.set_Node(this->get_Root());
  elem.train(data, this->splitting_Operator, this->splitting_Criteria,
             this->max_Depth, treshold);

  this->set_Root(std::make_unique<TreeNode>(*elem.node));
}

//
void tree_Prediction(const DataSet &data, std::vector<double> &result,
                     const std::vector<int> &index, TreeNode *node) {

  // Update the values of the result
  double pred_Val = node->get_Predicted_Value();

#pragma omp parallel for
  for (int idx : index) {
    result[idx] = pred_Val;
  }

  // Put the correct indexes
  auto [left_Index, right_Index] =
      data.split(node->get_Split_Column(), node->get_Split_Criterion(), index);

  if (node->get_Left_Node() && left_Index) {
    tree_Prediction(data, result, *left_Index, node->get_Left_Node());
  }

  if (node->get_Right_Node() && right_Index) {
    tree_Prediction(data, result, *right_Index, node->get_Right_Node());
  }
}

//
std::vector<double> DecisionTree::predict(const DataSet &data) const {

  int size = data.samples_Number();
  std::vector<double> result(size, 0);

  // Computes the index
  std::vector<int> index(size);
  for (int i = 0; i < size; ++i) {
    index[i] = i;
  }

  std::vector<double> tree_Result(size, 0);
  tree_Prediction(data, result, index, this->root.get());

  return result;
}
