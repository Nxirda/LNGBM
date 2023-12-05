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

  // Values used to parse the test dataset
  float split_Criteria = -1;
  int split_Column = -1;
  float predicted_Value = -1;

public:
  // Constructor

  TreeNode();
  TreeNode(std::shared_ptr<DataSet> data);
  TreeNode(std::shared_ptr<DataSet> data, std::vector<int> &idx);
  TreeNode &operator=(TreeNode const &tn); // copy assignment

  // Destructor

  ~TreeNode();

  // Setter

  void set_Index(std::vector<int> idx);
  void set_DataSet(std::shared_ptr<DataSet> data);

  void set_Split_Criteria(float criteria);
  void set_Split_Column(int col);
  void set_Predicted_Value(float value);

  // Getters

  std::shared_ptr<DataSet> get_DataSet();
  std::vector<int> get_Index();

  float get_Split_Criteria();
  int get_Split_Column();
  float get_Predicted_Value();

  // Methods

  float compute_Predicted_Value();
  void node_Print();
  void node_Print_Criteria();
  float node_Variance();
  bool node_Homogeneity();
  float node_Column_Mean(int position);
  std::vector<std::vector<int>> node_Split(int position, float split_Criteria);
};

#endif