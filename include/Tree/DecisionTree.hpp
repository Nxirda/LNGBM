#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "TreeNode.hpp"

// Binary Search Tree Class
class DecisionTree {
protected:
  // Parameters

  std::unique_ptr<TreeNode> root;

public:
  // Constructor

  DecisionTree();
  DecisionTree(const DecisionTree &dt);
  DecisionTree &operator=(const DecisionTree &tree);

  // Destructor

  ~DecisionTree();

  // Getters

  TreeNode *get_Root() const;

  // Setters
  void set_Root(std::unique_ptr<TreeNode> node);

  // Methods

  void print_Tree();
};

#endif