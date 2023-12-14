#ifndef TREE_NODE_H_
#define TREE_NODE_H_

#include <iostream>
#include <memory>

#include "DataSet.hpp"

// Node Class
class TreeNode {
protected:
  // Parameters
  std::unique_ptr<TreeNode> right;
  std::unique_ptr<TreeNode> left;
  
  // Values used to parse the test dataset
  int split_Column = -1;
  float split_Criterion = -1;
  float predicted_Value = -1;

public:
  // Constructor

  TreeNode();
  TreeNode(int split_Column, float split_Criterion, float predicted_Value);
  
  TreeNode(const TreeNode &node);
  TreeNode &operator=(const TreeNode &tn); // copy assignment

  // Destructor

  ~TreeNode();

  // Setter

  void set_Split_Column(int col);
  void set_Predicted_Value(float value);
  void set_Split_Criterion(float criterion);
  
  void add_Left(std::unique_ptr<TreeNode> Node);
  void add_Right(std::unique_ptr<TreeNode> Node);

  // Getters

  TreeNode *get_Left_Node() const;
  TreeNode *get_Right_Node() const;

  int get_Split_Column() const;
  float get_Split_Criterion() const;
  float get_Predicted_Value() const;

  // Methods

  void node_Print();
  void node_Print_Criterion();

};

#endif