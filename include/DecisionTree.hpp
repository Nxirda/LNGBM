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

  DecisionTree *parent;
  std::unique_ptr<DecisionTree> left;
  std::unique_ptr<DecisionTree> right;
  std::shared_ptr<TreeNode> curr_Node;

  IOperator *split_Operator = nullptr;
  std::shared_ptr<std::vector<float>> predicted_Labels = nullptr;

public:
  // Constructor

  DecisionTree();
  DecisionTree(const DataSet &data);
  DecisionTree(std::shared_ptr<TreeNode> tree_Node, std::vector<int> idx);

  // Destructor

  ~DecisionTree();

  // Getters

  DecisionTree *get_Left_Tree();
  DecisionTree *get_Right_Tree();
  DecisionTree *get_Parent_Tree();
  std::shared_ptr<TreeNode> get_Current_Node();
  std::shared_ptr<std::vector<float>> get_Predicted_Labels();

  // Setters

  void add_Parent(DecisionTree *dt);
  void add_Operator(IOperator *wanted_Operator);
  void add_Left(std::unique_ptr<DecisionTree> dt);
  void add_Right(std::unique_ptr<DecisionTree> dt);
  void
  add_Predicted_Labels(std::shared_ptr<std::vector<float>> predicted_Labels);

  void set_Test_DataSet(std::shared_ptr<DataSet> data);

  // Methods

  void print_Tree();

  void parse_Test_DataSet();
  void predict_Test_Labels();

  void build_Splitted_Tree(int depth);

  void divide_Predicted_Labels(int n);
  void sum_Predicted_Labels(DecisionTree *dt);

  std::vector<int> bootstrap_DataSet();
};

#endif