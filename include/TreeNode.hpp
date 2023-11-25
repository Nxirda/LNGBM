#include <iostream>
#include <memory>

#include "DataSet.hpp"

#ifndef TREE_NODE_H_
#define TREE_NODE_H_

// Node Class
class TreeNode {
protected:
  // Parameters

  std::shared_ptr<DataSet> data;
  std::vector<int> index;

public:
  // Constructor

  TreeNode();
  TreeNode(std::shared_ptr<DataSet> data);
  TreeNode(const std::shared_ptr<DataSet> &data, std::vector<int> &idx);
  TreeNode &operator=(TreeNode const &tn); // copy assignment

  // Destructor

  ~TreeNode();

  // Setter

  void set_Index(std::vector<int> idx);

  // Getters

  DataSet &get_DataSet();
  std::vector<int> get_Index();

  // Methods

  void node_Print();
  float node_Variance();
  bool node_Homogeneity();
  float node_Column_Mean(int position);
  std::vector<std::vector<int>> node_Split(int position, float split_Criteria);
};

#endif