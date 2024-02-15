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
Parameters :
Inputs     :
Outputs    : Object of Decision Tree Class
*/
DecisionTree::DecisionTree() { this->root = std::make_unique<TreeNode>(); }

/*
Constructor to copy a Tree
Parameters : Decision Tree
Inputs     : const DecisionTree
Outputs    : Object of Decision Tree Class
*/
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
TreeNode *DecisionTree::get_Root() const{ return this->root.get(); }

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