#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "DataSet.hpp"
#include "TreeNode.hpp"

#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

// Binary Search Tree Class
class DecisionTree {
protected:
  // Parameters

  DecisionTree *parent;
  std::shared_ptr<TreeNode> curr_Node;
  std::unique_ptr<DecisionTree> right;
  std::unique_ptr<DecisionTree> left;

public:
  // Constructor

  DecisionTree();
  DecisionTree(const DataSet &data);
  DecisionTree(const std::shared_ptr<DataSet> data, std::vector<int> idx);
  //DecisionTree(const DecisionTree &dt);      // Copy operator
  DecisionTree &operator=(DecisionTree &dt); // Copy assignment

  // Destructor

  ~DecisionTree();

  // Getters

  TreeNode &get_Current_Node();
  DecisionTree &get_Parent_Tree();
  DecisionTree *get_Right_Tree();
  DecisionTree *get_Left_Tree();

  // Setters

  void add_Right(std::unique_ptr<DecisionTree> dt);
  void add_Left(std::unique_ptr<DecisionTree> dt);
  void add_Parent(DecisionTree *d);

  // Methods

  void build_Splitted_Tree(int depth);
  int find_Best_Split_Feature();
  void print_Tree();

private:
  // Methods only used inside the class

  float splitting_Variance(int position);
};

#endif
