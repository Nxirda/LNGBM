#ifndef TREE_NODE_H_
#define TREE_NODE_H_

#include <iostream>
#include <memory>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/unique_ptr.hpp>

#include "DataSet.hpp"

// Node Class
class TreeNode {
private:
  // Parameters

  // Values used to parse the test dataset
  double split_Criterion = -1.0;
  double predicted_Value = -1.0;
  size_t split_Column = 0;

  std::unique_ptr<TreeNode> right;
  std::unique_ptr<TreeNode> left;

  // Boost part to serialize the nodes so we can send them to other MPIs Process
  // Might need to build some sort of verif for MPI/Boost install at some point
  // so we can use it with or without them
  friend class boost::serialization::access;
  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    // Unused param but necessary for boost
    static_cast<void>(version);
    ar &split_Column;
    ar &split_Criterion;
    ar &predicted_Value;
    ar &left;
    ar &right;
  }

public:
  // Constructor

  TreeNode() noexcept;
  TreeNode(size_t split_Column, double split_Criterion,
           double predicted_Value) noexcept;

  TreeNode(const TreeNode &node) noexcept;
  TreeNode &operator=(const TreeNode &tn) noexcept; // copy assignment

  // Move semantic part
  TreeNode(TreeNode &&node) noexcept;
  TreeNode &operator=(TreeNode &&tn) noexcept;

  // Destructor

  ~TreeNode();

  // Setter

  void set_Split_Column(size_t col);
  void set_Predicted_Value(double value);
  void set_Split_Criterion(double criterion);

  void add_Left(std::unique_ptr<TreeNode> Node);
  void add_Right(std::unique_ptr<TreeNode> Node);

  // Getters

  TreeNode *get_Left_Node() const;
  TreeNode *get_Right_Node() const;

  size_t get_Split_Column() const;
  double get_Split_Criterion() const;
  double get_Predicted_Value() const;

  // Methods

  // void node_Print();
};

#endif