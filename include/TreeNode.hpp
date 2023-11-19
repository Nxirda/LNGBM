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
  TreeNode(std::shared_ptr<DataSet> data, std::vector<int> &idx);
  TreeNode &operator=(TreeNode const &tn); // copy assignment

  // Destructor

  ~TreeNode();

  // Getters

  DataSet &get_DataSet();

  // Methods

  float node_Variance();
  bool node_Homogeneity();
};

#endif