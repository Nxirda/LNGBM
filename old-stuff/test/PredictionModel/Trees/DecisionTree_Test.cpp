#include <DecisionTree.hpp>
#include <gtest/gtest.h>

//
TEST(DecisionTreeTest, DefaultConstructor) {
  DecisionTree tree{};
  ASSERT_NE(tree.get_Root(), nullptr);
} 

//
TEST(DecisionTreeTest, CopyConstructor) {
  DecisionTree tree1;
  tree1.set_Root(std::make_unique<TreeNode>(1, 1.5, 2.0));

  DecisionTree tree2(tree1);

  ASSERT_NE(tree2.get_Root(), nullptr);
  ASSERT_EQ(tree2.get_Root()->get_Split_Column(), 1);
}

//
TEST(DecisionTreeTest, AssignmentOperator) {
  DecisionTree tree1;
  tree1.set_Root(std::make_unique<TreeNode>(1, 1.5, 2.0));
  DecisionTree tree2;
  tree2 = tree1;
  ASSERT_NE(tree2.get_Root(), nullptr);
  ASSERT_EQ(tree2.get_Root()->get_Split_Column(), 1);
}

//
TEST(DecisionTreeTest, GetRoot) {
  DecisionTree tree;
  tree.set_Root(std::make_unique<TreeNode>(1, 1.5, 2.0));
  ASSERT_NE(tree.get_Root(), nullptr);
}

//
TEST(DecisionTreeTest, SetRoot) {
  DecisionTree tree;
  tree.set_Root(std::make_unique<TreeNode>(2, 2.5, 3.0));
  ASSERT_NE(tree.get_Root(), nullptr);
  ASSERT_EQ(tree.get_Root()->get_Split_Column(), 2);
}