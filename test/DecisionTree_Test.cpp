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
    DecisionTree tree1;
    tree1.set_Root(new TreeNode(1, 1.5, 2.0));
    DecisionTree tree2(tree1);
    EXPECT_NE(tree2.get_Root(), nullptr);
    EXPECT_EQ(tree2.get_Root()->getSplitColumn(), 1);
}

TEST_F(DecisionTreeTest, AssignmentOperator) 
{
    DecisionTree tree1;
    tree1.set_Root(new TreeNode(1, 1.5, 2.0));
    DecisionTree tree2;
    tree2 = tree1;
    EXPECT_NE(tree2.get_Root(), nullptr);
    EXPECT_EQ(tree2.get_Root()->getSplitColumn(), 1);
}

TEST_F(DecisionTreeTest, Destructor) 
{
    DecisionTree* tree = new DecisionTree();
    tree->set_Root(new TreeNode());
    delete tree;

}

TEST_F(DecisionTreeTest, GetRoot) 
{
    DecisionTree tree;
    tree.set_Root(new TreeNode(1, 1.5, 2.0));
    EXPECT_NE(tree.get_Root(), nullptr);
}

TEST_F(DecisionTreeTest, SetRoot) 
{
    DecisionTree tree;
    tree.set_Root(new TreeNode(2, 2.5, 3.0));
    EXPECT_NE(tree.get_Root(), nullptr);
    EXPECT_EQ(tree.get_Root()->getSplitColumn(), 2);
}

TEST_F(DecisionTreeTest, InsertNode) 
{
    DecisionTree tree;
    tree.insert(3, 3.5, 4.0);
    EXPECT_NE(tree.get_Root(), nullptr);
}

TEST_F(DecisionTreeTest, DeleteNode) 
{
    DecisionTree tree;
    tree.insert(3, 3.5, 4.0);
    tree.delete(3);

}

TEST_F(DecisionTreeTest, SearchNode) 
{
    DecisionTree tree;
    tree.insert(4, 4.5, 5.0);
    TreeNode* node = tree.search(4);
    EXPECT_NE(node, nullptr);

}

TEST_F(DecisionTreeTest, TraverseTree) 
{
    DecisionTree tree;
    tree.insert(5, 5.5, 6.0);
    std::vector<TreeNode*> nodes = tree.inorder();
    EXPECT_FALSE(nodes.empty());

}




