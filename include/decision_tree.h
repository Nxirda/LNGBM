#include <iostream>
#include <string>
#include <vector>

#include "data_loading.h"

#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

// Node Class
class TreeNode {
protected:
  // Parameters
  Dataset data;
  // Division Function to be added

public:
  // Constructor
  TreeNode();
  TreeNode(const Dataset &d);
  TreeNode &operator=(TreeNode const &TN); // copy assignment

  // Getters
  Dataset get_Dataset();

  // Methods
};

// Binary Search Tree Class
class DecisionTree {
protected:
  // Parameters
  DecisionTree *Parent;
  TreeNode *Curr_Node; // Actual Node with properties
  DecisionTree *Right; // Right Node pointer
  DecisionTree *Left;  // Left  Node pointer

public:
  // Constructor
  DecisionTree(const Dataset &data);
  DecisionTree &operator=(const DecisionTree &DT);

  // Getters
  TreeNode &get_Current_Node();
  DecisionTree &get_ParentTree();
  DecisionTree &get_RightTree();
  DecisionTree &get_LeftTree();

  // Setters
  void add_right(Dataset data);
  void add_left(Dataset data);
  void add_Parent(DecisionTree *d);

  // Methods
  DecisionTree *buildTree();

  //Methods Algo Splitting
  void print_Tree();
  void Algo_Splitting(DecisionTree* tree);
  bool Stop_Condition(const Dataset& data);
  double computeGiniIndex(const Dataset& data, const std::string& attribute);
  std::string chooseSplitAttribute(const Dataset& data);
  std::vector<Dataset> splitByAttribute(const Dataset& data, const std::string& attribute);
  
};

#endif
