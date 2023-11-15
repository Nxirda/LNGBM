#include <iostream>
#include <string>
#include <vector>

#include "data_loading.h"

#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

// Node Class
class TreeNode {
protected:
  // Parameters

  Dataset data;

public:
  // Constructor

  TreeNode();
  TreeNode(const Dataset &d);
  TreeNode &operator=(TreeNode const &TN); // copy assignment

  // Destructor

  ~TreeNode();

  // Getters

  Dataset get_Dataset();

  // Methods

  float NodeVariance();
  bool NodeHomogeneity();
};

// Binary Search Tree Class
class DecisionTree {
protected:
  // Parameters

  DecisionTree *Parent;
  TreeNode *Curr_Node; 
  DecisionTree *Right; 
  DecisionTree *Left;  

public:
  // Constructor

  DecisionTree(const Dataset &data);
  DecisionTree &operator=(const DecisionTree &DT);

  // Destructor

  ~DecisionTree();

  // Getters

  TreeNode &get_Current_Node();
  DecisionTree &get_ParentTree();
  DecisionTree &get_RightTree();
  DecisionTree &get_LeftTree();

  // Setters

  void add_right(Dataset data);
  void add_left(Dataset data);
  void add_Parent(DecisionTree *d);

  // Methods
<<<<<<< HEAD

  void Build_Splitted_Tree(DecisionTree *DT);
  std::string FindBestAttribute();
  void print_Tree();

private:
  // Methods only used inside the class

  float splitting_variance(int position);
=======
  DecisionTree *buildTree();

  //Methods Algo Splitting
  void print_Tree();
  void Algo_Splitting(DecisionTree* tree);
  bool Stop_Condition(const Dataset& data);
  double computeGiniIndex(const Dataset& data, const std::string& attribute);
  std::string chooseSplitAttribute(const Dataset& data);
  std::vector<Dataset> splitByAttribute(const Dataset& data, const std::string& attribute);
  
>>>>>>> Valentin
};

#endif
