#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "data_loading.h"

#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

// Node Class
class TreeNode {
protected:
  // Parameters

  DataSet data;

public:
  // Constructor

  TreeNode();
  TreeNode(const DataSet &d);
  TreeNode &operator=(TreeNode const& tn); // copy assignment

  // Destructor

  ~TreeNode();

  // Getters

  DataSet get_DataSet();

  // Methods

  float node_Variance();
  bool node_Homogeneity();
};

// Binary Search Tree Class
class DecisionTree {
protected:
  // Parameters

  std::shared_ptr<DecisionTree> parent; //Shared with at least two son Trees (for the top Node) might wanna recast it to be naked ptr
  TreeNode *curr_Node; 
  std::unique_ptr<DecisionTree> right; 
  std::unique_ptr<DecisionTree> left;  

public:
  // Constructor

  DecisionTree();
  DecisionTree(const DataSet &data);
  DecisionTree(const DecisionTree &dt);      //Copy oerator
  DecisionTree &operator=(DecisionTree &dt); //Copy assignment

  // Destructor

  ~DecisionTree();

  // Getters

  TreeNode &get_Current_Node();
  std::shared_ptr<DecisionTree> get_Parent_Tree();
  DecisionTree &get_Right_Tree();
  DecisionTree &get_Left_Tree();

  // Setters

  void add_Right(std::unique_ptr<DecisionTree> dt);
  void add_Left(std::unique_ptr<DecisionTree> dt);
  void add_Parent(std::shared_ptr<DecisionTree> d);

  // Methods

  void build_Splitted_Tree(DecisionTree *dt);
  std::string find_Best_Feature();
  void print_Tree();

private:
  // Methods only used inside the class

  float splitting_Variance(int position);
};

#endif
