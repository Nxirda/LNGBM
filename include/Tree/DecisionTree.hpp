#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "DataSet.hpp"
#include "IOperator.hpp"
#include "TreeNode.hpp"

// Binary Search Tree Class
class DecisionTree {
protected:
  // Parameters

  std::unique_ptr<TreeNode> root;
  std::shared_ptr<std::vector<float>> predicted_Labels = nullptr;

public:
  // Constructor

  DecisionTree();
  DecisionTree &operator=(const DecisionTree &tree);

  // Destructor

  ~DecisionTree();

  // Getters

  TreeNode *get_Root();
  std::shared_ptr<std::vector<float>> get_Predicted_Labels();

  // Setters
  void set_Root(std::unique_ptr<TreeNode> node);
  void
  add_Predicted_Labels(std::shared_ptr<std::vector<float>> predicted_Labels);

  // Methods

  void print_Tree();

  void parse_Test_DataSet();
  void predict_Test_Labels();

  void train_Tree(const DataSet &data, IOperator* splitting_Operator, int max_Depth);

};

#endif