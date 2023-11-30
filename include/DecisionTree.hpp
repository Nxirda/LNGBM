#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "DataSet.hpp"
#include "IOperator.hpp"
#include "TreeNode.hpp"

#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

// Binary Search Tree Class
class DecisionTree {
protected:
  // Parameters
  IOperator *split_Operator = nullptr;
  DecisionTree *parent;
  std::shared_ptr<TreeNode> curr_Node;
  std::unique_ptr<DecisionTree> right;
  std::unique_ptr<DecisionTree> left;

public:
  // Constructor

  DecisionTree();
  DecisionTree(const DataSet &data);
  DecisionTree(std::shared_ptr<TreeNode> tree_Node, std::vector<int> idx);

  // Destructor

  ~DecisionTree();

  // Getters

  std::shared_ptr<TreeNode> get_Current_Node();
  DecisionTree &get_Parent_Tree();
  DecisionTree *get_Right_Tree();
  DecisionTree *get_Left_Tree();

  // Setters

  void add_Right(std::unique_ptr<DecisionTree> dt);
  void add_Left(std::unique_ptr<DecisionTree> dt);
  void add_Parent(DecisionTree *d);
  void add_Operator(IOperator *wanted_Operator);

  // Methods

  void build_Splitted_Tree(int depth);
  void print_Tree();

};

#endif
