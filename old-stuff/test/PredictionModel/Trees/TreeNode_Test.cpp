#include <TreeNode.hpp>
#include <gtest/gtest.h>

//
TEST(TreeNodeTest, DefaultConstructor) {
  TreeNode default_Node{};
  ASSERT_EQ(default_Node.get_Split_Column(), 0);
  ASSERT_EQ(default_Node.get_Split_Criterion(), -1);
  ASSERT_EQ(default_Node.get_Predicted_Value(), -1);
}

//
TEST(TreeNodeTest, ParameterizedConstructor) {
  TreeNode custom_Node{1, 2.5, 3.0};
  ASSERT_EQ(custom_Node.get_Split_Column(), 1);
  ASSERT_EQ(custom_Node.get_Split_Criterion(), 2.5);
  ASSERT_EQ(custom_Node.get_Predicted_Value(), 3.0);
}

//
TEST(TreeNodeTest, AddChildNode) {
  TreeNode custom_Node{1, 2.5, 3.0};

  // Sub child node
  TreeNode child_node{2, 3.5, 4.0};
  custom_Node.add_Left(std::make_unique<TreeNode>(child_node));
  TreeNode right_child_node{3, 4.5, 5.0};
  custom_Node.add_Right(std::make_unique<TreeNode>(right_child_node));

  TreeNode *left_child = custom_Node.get_Left_Node();
  ASSERT_NE(left_child, nullptr);
  ASSERT_EQ(left_child->get_Split_Column(), 2);
  ASSERT_EQ(left_child->get_Split_Criterion(), 3.5);
  ASSERT_EQ(left_child->get_Predicted_Value(), 4.0);

  TreeNode *right_child = custom_Node.get_Right_Node();
  ASSERT_NE(right_child, nullptr);
  ASSERT_EQ(right_child->get_Split_Column(), 3);
  ASSERT_EQ(right_child->get_Split_Criterion(), 4.5);
  ASSERT_EQ(right_child->get_Predicted_Value(), 5.0);
}

//
TEST(TreeNodeTest, ValueValidation) {
  TreeNode invalid_node{0, -1.0, -1.0};

  ASSERT_EQ(invalid_node.get_Split_Column(), 0);
  ASSERT_EQ(invalid_node.get_Split_Criterion(), -1.0);
  ASSERT_EQ(invalid_node.get_Predicted_Value(), -1.0);
}