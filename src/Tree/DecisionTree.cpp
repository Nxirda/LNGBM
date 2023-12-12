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
DecisionTree::DecisionTree() { this->root = std::make_unique<TreeNode>(); }

/**/
DecisionTree::DecisionTree(const DecisionTree &dt){
  this->root = std::make_unique<TreeNode>(*dt.get_Root());
}

/**/
DecisionTree &DecisionTree::operator=(const DecisionTree &tree) {
  this->root = std::make_unique<TreeNode>(*tree.get_Root());
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
TreeNode *DecisionTree::get_Root() const{ return this->root.get(); }

/**/
void DecisionTree::set_Root(std::unique_ptr<TreeNode> node) {
  this->root = std::move(node);
}

/**/
void DecisionTree::print_Tree() { this->root->node_Print_Criterion(); }