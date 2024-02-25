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

//
DecisionTree::DecisionTree() {
  this->root = std::make_unique<TreeNode>();
  this->splitting_Criteria = nullptr;
  this->splitting_Operator = nullptr;
  this->max_Depth = 0;
}

//
DecisionTree::DecisionTree(uint16_t max_Depth, ICriteria *crit, IOperator *op) {
  this->root = std::make_unique<TreeNode>();
  this->splitting_Criteria = crit;
  this->splitting_Operator = op;
  this->max_Depth = max_Depth;
}

//
DecisionTree::DecisionTree(const DecisionTree &dt) {
  this->root = std::make_unique<TreeNode>(*dt.get_Root());
  this->splitting_Criteria = dt.splitting_Criteria;
  this->splitting_Operator = dt.splitting_Operator;
}

//
DecisionTree &DecisionTree::operator=(const DecisionTree &tree) {
  this->root = std::make_unique<TreeNode>(*tree.get_Root());
  this->splitting_Criteria = tree.splitting_Criteria;
  this->splitting_Operator = tree.splitting_Operator;
  return *this;
}

//
DecisionTree::~DecisionTree(){};

//
TreeNode *DecisionTree::get_Root() const { return this->root.get(); }

//
void DecisionTree::set_Root(std::unique_ptr<TreeNode> node) {
  this->root = std::move(node);
}

//
void DecisionTree::print_Tree() { this->root->node_Print(); }

//
void DecisionTree::train(const DataSet &data) {
  size_t threshold = 5;

  TrainingElement::train(data, this->get_Root(), this->splitting_Operator,
                         this->splitting_Criteria, this->max_Depth, threshold);
}

//
void tree_Prediction(const DataSet &data, std::vector<double> &result,
                     const std::vector<size_t> &index, TreeNode *node) {

  // Update the values of the result
  double pred_Val = node->get_Predicted_Value();

  for (size_t idx : index) {
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

  size_t size = data.samples_Number();
  std::vector<double> result(size, 0);

  // Computes the index
  std::vector<size_t> index(size);
  for (size_t i = 0; i < size; ++i) {
    index[i] = i;
  }

  std::vector<double> tree_Result(size, 0);
  tree_Prediction(data, result, index, this->root.get());

  return result;
}
