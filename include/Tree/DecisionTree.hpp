#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "TreeNode.hpp"

// Binary Search Tree Class
class DecisionTree {
private:
  // Parameters

  std::unique_ptr<TreeNode> root;

  // Boost part to serialize the trees so we can send them to other MPIs Process
  // Might need to build some sort of verif for MPI/Boost install at some point
  // so we can use it with or without them
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &root;
  }

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

  //Add a train   method
  //Add a predict method

  void print_Tree();
};

#endif