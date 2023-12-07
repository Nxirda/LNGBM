#include <iostream>
#include <memory>

#include "DataSet.hpp"

#ifndef TREE_NODE_H_
#define TREE_NODE_H_

// Node Class
class TreeNode {
protected:
  // Parameters

  std::vector<int> index;
  std::shared_ptr<DataSet> data;

  // Values used to parse the test dataset
  int split_Column = -1;
  float split_Criteria = -1;
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

  void set_Split_Column(int col);
  void set_Predicted_Value(float value);
  void set_Split_Criteria(float criteria);

  // Getters

  std::vector<int> get_Index();
  std::shared_ptr<DataSet> get_DataSet();

  int get_Split_Column();
  float get_Split_Criteria();
  float get_Predicted_Value();

  // Methods

  void node_Print();
  void node_Print_Criteria();

  float node_Variance();
  bool node_Homogeneity();
  float compute_Predicted_Value();
  float node_Column_Mean(int position);
  std::vector<int> bootstrap_DataSet();
  std::vector<std::vector<int>> node_Split(int position, float split_Criteria);
};

#endif