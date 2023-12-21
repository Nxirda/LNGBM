#include <DecisionTree.hpp> 
#include <gtest/gtest.h>

class DecisionTreeTest : public ::testing::Test 
{
protected:
    DecisionTree tree;
};

TEST_F(DecisionTreeTest, DefaultConstructor) 
{
    DecisionTree tree;
    EXPECT_EQ(tree.get_Root(), nullptr);
}

TEST_F(DecisionTreeTest, CopyConstructor) 
{
    std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>(1, 1.5, 2.0);
    DecisionTree tree1;
    tree1.set_Root(std::move(node));
    DecisionTree tree2(tree1);
    EXPECT_NE(tree2.get_Root(), nullptr);
    EXPECT_EQ(tree2.get_Root()->get_Split_Column(), 1);
}

TEST_F(DecisionTreeTest, AssignmentOperator) 
{
    std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>(1, 1.5, 2.0);
    DecisionTree tree1;
    tree1.set_Root(std::move(node));
    DecisionTree tree2;
    tree2 = tree1;
    EXPECT_NE(tree2.get_Root(), nullptr);
    EXPECT_EQ(tree2.get_Root()->get_Split_Column(), 1);
}

TEST_F(DecisionTreeTest, Destructor) 
{
    DecisionTree* tree = new DecisionTree();
    tree->set_Root(std::make_unique<TreeNode>());
    delete tree;
}

TEST_F(DecisionTreeTest, GetRoot) 
{
    std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>(1, 1.5, 2.0);
    DecisionTree tree;
    tree.set_Root(std::move(node));
    EXPECT_NE(tree.get_Root(), nullptr);
}

TEST_F(DecisionTreeTest, SetRoot) 
{
    std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>(2, 2.5, 3.0);
    DecisionTree tree;
    tree.set_Root(std::move(node));
    EXPECT_NE(tree.get_Root(), nullptr);
    EXPECT_EQ(tree.get_Root()->get_Split_Column(), 2);
}


TEST_F(DecisionTreeTest, TreeNodeOperations) 
{
    std::unique_ptr<TreeNode> root = std::make_unique<TreeNode>(1, 1.5, 2.0);
    std::unique_ptr<TreeNode> leftChild = std::make_unique<TreeNode>(2, 2.5, 3.0);
    std::unique_ptr<TreeNode> rightChild = std::make_unique<TreeNode>(3, 3.5, 4.0);
    
    root->add_Left(std::move(leftChild));
    root->add_Right(std::move(rightChild));

    DecisionTree tree;
    tree.set_Root(std::move(root));

    EXPECT_NE(tree.get_Root(), nullptr);
    EXPECT_NE(tree.get_Root()->get_Left_Node(), nullptr);
    EXPECT_NE(tree.get_Root()->get_Right_Node(), nullptr);
}

TEST_F(DecisionTreeTest, PrintTree) 
{
    DecisionTree tree;
    std::unique_ptr<TreeNode> node = std::make_unique<TreeNode>(1, 1.5, 2.0);
    tree.set_Root(std::move(node));
    EXPECT_NO_THROW(tree.print_Tree());
}




