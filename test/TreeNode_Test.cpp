#include <TreeNode.hpp>
#include <gtest/gtest.h>

class TreeNodeTest : public ::testing::Test {
protected:
    TreeNode default_node;
    TreeNode custom_node{1, 2.5, 3.0};
};

TEST_F(TreeNodeTest, DefaultConstructor) 
{
    EXPECT_EQ(default_node.get_Split_Column(), -1);
    EXPECT_EQ(default_node.get_Split_Criterion(), -1);
    EXPECT_EQ(default_node.get_Predicted_Value(), -1);
}

TEST_F(TreeNodeTest, ParameterizedConstructor) 
{
    EXPECT_EQ(custom_node.get_Split_Column(), 1);
    EXPECT_EQ(custom_node.get_Split_Criterion(), 2.5);
    EXPECT_EQ(custom_node.get_Predicted_Value(), 3.0);
}

TEST_F(TreeNodeTest, AddChildNode) 
{
    std::unique_ptr<TreeNode> child_node = std::make_unique<TreeNode>(2, 3.5, 4.0);
    custom_node.add_Left(std::move(child_node));

    TreeNode* left_child = custom_node.get_Left_Node();
    ASSERT_NE(left_child, nullptr);
    EXPECT_EQ(left_child->get_Split_Column(), 2);
    EXPECT_EQ(left_child->get_Split_Criterion(), 3.5);
    EXPECT_EQ(left_child->get_Predicted_Value(), 4.0);

    std::unique_ptr<TreeNode> right_child_node = std::make_unique<TreeNode>(3, 4.5, 5.0);
    custom_node.add_Right(std::move(right_child_node));

    TreeNode* right_child = custom_node.get_Right_Node();
    ASSERT_NE(right_child, nullptr); 
    EXPECT_EQ(right_child->get_Split_Column(), 3);
    EXPECT_EQ(right_child->get_Split_Criterion(), 4.5);
    EXPECT_EQ(right_child->get_Predicted_Value(), 5.0);
}

TEST_F(TreeNodeTest, RemoveChildNode) 
{
    custom_node.add_Left(nullptr);
    custom_node.add_Right(nullptr);

    TreeNode* left_child = custom_node.get_Left_Node();
    EXPECT_EQ(left_child, nullptr);

    TreeNode* right_child = custom_node.get_Right_Node();
    EXPECT_EQ(right_child, nullptr);
}

TEST_F(TreeNodeTest, ValueValidation) 
{
    TreeNode invalid_node{-1, -1.0, -1.0};

    ASSERT_TRUE(invalid_node.get_Split_Column() >= 0 || invalid_node.get_Split_Column() == -1);
    ASSERT_GE(invalid_node.get_Split_Criterion(), 0.0);
    ASSERT_GE(invalid_node.get_Predicted_Value(), 0.0);
}