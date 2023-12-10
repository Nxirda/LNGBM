#include <fstream>
#include <iostream>
#include <iterator>
#include <stack>
#include <string>
#include <vector>

#include "DecisionTree.hpp"
#include "TreeNode.hpp"

/********************/
/*                  */
/*    TREE CLASS    */
/*                  */
/********************/

/*
Default Constructor
Inputs  :
Outputs : Object of Decision Tree Class
*/
DecisionTree::DecisionTree() {
  this->root = std::make_unique<TreeNode>();
}

/**/
DecisionTree &DecisionTree::operator=(const DecisionTree &tree){
  this->root = std::make_unique<TreeNode>(*tree.root);
  return *this;
}

/*
Default Destructor
Inputs  :
Outputs :
*/
DecisionTree::~DecisionTree(){};

/*
Returns the Current Node of the Tree
Inputs  :
Outputs : pointer of Decision Tree Object
*/
TreeNode *DecisionTree::get_Root() {
  return this->root.get();
}

/**/
void DecisionTree::set_Root(std::unique_ptr<TreeNode> node){
  this->root = std::move(node);
}


/*
Returns the Predicted Labels vector
Inputs  :
Outputs : shared_ptr<vector<float>>
*/
std::shared_ptr<std::vector<float>> DecisionTree::get_Predicted_Labels() {
  return this->predicted_Labels;
}

/*
Sets the predicted labels for the given tree
Inputs  : shared_ptr<vector<float>>
Outputs :
*/
void DecisionTree::add_Predicted_Labels(
    std::shared_ptr<std::vector<float>> predicted_Labels) {
  this->predicted_Labels = predicted_Labels;
}

void DecisionTree::print_Tree(){
  this->root->node_Print_Criterion();
}

/* 
void DecisionTree::train_Tree(const DataSet &data,
                              IOperator *splitting_Operator, int max_Depth) {
  std::stack<TrainingElement> remaining;
  TrainingElement first_Elem{};

  remaining.push(first_Elem.build_Root(data.labels_Number(), this));

  while (not remaining.empty()) {

    auto elem = remaining.top();
    remaining.pop();

    if (elem.depth >= max_Depth) {
      continue;
    }
    auto [left, right] = elem.split_Node(data, &elem, splitting_Operator);
    if (left) {
      remaining.push(*left);
    }
    if (right) {
      remaining.push(*right);
    }
  }
} */

/*
Computes the values predictions for the test DataSet
and store them in the predicted_Label parameter
Inputs  :
Outputs :
*/
/* void DecisionTree::predict_Test_Labels() {

  int size = this->get_Predicted_Labels()->size();

  if (this->root->get_Predicted_Value() >= 0) {
    for (auto idx : this->root->get_Index()) {
      if (idx < size) {
        this->predicted_Labels->at(idx) =
            this->root->get_Predicted_Value();
      }
    }
  }
   if (this->get_Left_Tree()) {
    this->left->predict_Test_Labels();
  }
  if (this->get_Right_Tree()) {
    this->right->predict_Test_Labels();
  } 
} */
