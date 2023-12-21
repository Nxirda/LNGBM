#include <TreeNode.hpp>
#include <gtest/gtest.h>

class TreeNodeTest : public ::testing::Test {
protected:
    TreeNode default_node;
    TreeNode custom_node{1, 2.5, 3.0};
};

TEST_F(TreeNodeTest, DefaultConstructor) 
{
    EXPECT_EQ(default_node.getSplitColumn(), -1);
    EXPECT_EQ(default_node.getSplitCriterion(), -1);
    EXPECT_EQ(default_node.getPredictedValue(), -1);
}

TEST_F(TreeNodeTest, ParameterizedConstructor) 
{
    EXPECT_EQ(custom_node.getSplitColumn(), 1);
    EXPECT_EQ(custom_node.getSplitCriterion(), 2.5);
    EXPECT_EQ(custom_node.getPredictedValue(), 3.0);
}

TEST_F(TreeNodeTest, AddChildNode) 
{
    TreeNode child_node{2, 3.5, 4.0};
    custom_node.setLeftChild(std::make_unique<TreeNode>(child_node));

    TreeNode* left_child = custom_node.getLeftChild();
    ASSERT_NE(left_child, nullptr);
    EXPECT_EQ(left_child->getSplitColumn(), 2);
    EXPECT_EQ(left_child->getSplitCriterion(), 3.5);
    EXPECT_EQ(left_child->getPredictedValue(), 4.0);

    TreeNode right_child_node{3, 4.5, 5.0};
    custom_node.setRightChild(std::make_unique<TreeNode>(right_child_node));

    TreeNode* right_child = custom_node.getRightChild();
    ASSERT_NE(right_child, nullptr); 
    EXPECT_EQ(right_child->getSplitColumn(), 3);
    EXPECT_EQ(right_child->getSplitCriterion(), 4.5);
    EXPECT_EQ(right_child->getPredictedValue(), 5.0);
}

TEST_F(TreeNodeTest, RemoveChildNode) 
{
    custom_node.setLeftChild(nullptr);
    custom_node.setRightChild(nullptr);

    TreeNode* left_child = custom_node.getLeftChild();
    EXPECT_EQ(left_child, nullptr);

    TreeNode* right_child = custom_node.getRightChild();
    EXPECT_EQ(right_child, nullptr);

}

TEST_F(TreeNodeTest, ValueValidation) 
{
    TreeNode invalid_node{-1, -1.0, -1.0};

    ASSERT_TRUE(invalid_node.getSplitColumn() >= 0 || invalid_node.getSplitColumn() == -1);
    ASSERT_GE(invalid_node.getSplitCriterion(), 0.0);
    ASSERT_GE(invalid_node.getPredictedValue(), 0.0);
}