#include <fstream>
#include <iostream>
#include <iterator>
#include <limits.h> //For Max_Int value
#include <string>
#include <vector>

#include "DecisionTree.hpp"
#include "TreeNode.hpp"

/********************/
/*                  */
/*    TREE CLASS    */
/*                  */
/********************/

/*
Default Constructor
Inputs  :
Outputs : Object of Decision Tree Class
*/
DecisionTree::DecisionTree() {
  this->curr_Node = std::make_shared<TreeNode>();
  this->parent = nullptr;
  this->left = nullptr;
  this->right = nullptr;
}

/*
Base Constructor with a DataSet parameter
Shall be used only for the first Node
Inputs  : Object of DataSet Class
Outputs : Object of Decision Tree Class
*/
DecisionTree::DecisionTree(const DataSet &data) {
  std::vector<int> idx(data.samples_Number() - 1);
  std::vector<float> test(0, 0);

  for (long unsigned int i = 0; i < idx.size(); ++i) {
    idx[i] = i;
  }
  this->parent = nullptr;
  this->curr_Node = std::make_shared<TreeNode>(
      TreeNode{std::make_shared<DataSet>(data), idx});
  this->left = nullptr;
  this->right = nullptr;
  this->predicted_Labels = std::make_shared<std::vector<float>>(test);
}

/*
Constructor for child nodes
Inputs  : shared_ptr<DataSet>, vector<int>
Outputs : Object of Decision Tree Class
*/
DecisionTree::DecisionTree(std::shared_ptr<TreeNode> tree_Node,
                           std::vector<int> idx) {
  this->parent = nullptr;
  this->curr_Node = tree_Node;
  this->curr_Node.get()->set_Index(idx);
  this->left = nullptr;
  this->right = nullptr;
}

/*
Default Destructor
Inputs  :
Outputs :
*/
DecisionTree::~DecisionTree(){};

/*
Returns the Current Node of the Tree
Inputs  :
Outputs : pointer of Decision Tree Object
*/
std::shared_ptr<TreeNode> DecisionTree::get_Current_Node() {
  return this->curr_Node;
}

/*
Returns the Parent Tree
Inputs  :
Outputs : pointer of Decision Tree Object
*/
DecisionTree *DecisionTree::get_Parent_Tree() { return this->parent; }

/*
Returns the Left Sub Tree
Inputs  :
Outputs : pointer of Decision Tree Object
*/
DecisionTree *DecisionTree::get_Left_Tree() { return this->left.get(); }

/*
Returns the Right Sub Tree
Inputs  :
Outputs : pointer of Decision Tree Object
*/
DecisionTree *DecisionTree::get_Right_Tree() { return this->right.get(); }

/*
Returns the Predicted Labels vector
Inputs  :
Outputs : shared_ptr<vector<float>>
*/
std::shared_ptr<std::vector<float>> DecisionTree::get_Predicted_Labels() {
  return this->predicted_Labels;
}

/*
Sets a new Parent for the given tree
Inputs  : pointer of Decision Tree Object
Outputs :
*/
void DecisionTree::add_Parent(DecisionTree *d) { this->parent = d; }

/*
Sets the predicted labels for the given tree
Inputs  : shared_ptr<vector<float>>
Outputs :
*/
void DecisionTree::add_Predicted_Labels(
    std::shared_ptr<std::vector<float>> predicted_Labels) {
  this->predicted_Labels = predicted_Labels;
}

/*
Sets a new left Subtree
Inputs : Object of DataSet class
Output :
*/
void DecisionTree::add_Left(std::unique_ptr<DecisionTree> dt) {
  this->left = std::move(dt);
  // this->left->curr_Node = std::make_shared<TreeNode>(
  //     this->curr_Node->get_DataSet(), dt->get_Current_Node()->get_Index());
  this->left->add_Parent(this);
  this->left->add_Operator(this->split_Operator);
  this->left->add_Predicted_Labels(this->get_Predicted_Labels());
}

/*
Sets a new right Subtree
Inputs : Object of DataSet class
Output :
*/
void DecisionTree::add_Right(std::unique_ptr<DecisionTree> dt) {
  this->right = std::move(dt);
  // this->right->curr_Node = std::make_shared<TreeNode>(
  //     this->curr_Node->get_DataSet(), dt->get_Current_Node()->get_Index());
  this->right->add_Parent(this);
  this->right->add_Operator(this->split_Operator);
  this->right->add_Predicted_Labels(this->get_Predicted_Labels());
}

void DecisionTree::add_Operator(IOperator *wanted_Operator) {
  this->split_Operator = wanted_Operator;
}

/*
Print function for Decision Trees
Inputs  :
Outputs :
*/
void DecisionTree::print_Tree() {
  // this->curr_Node->node_Print();
  // std::cout << this->curr_Node->get_Index().size();
  this->curr_Node->node_Print_Criteria();
  if (this->left) {
    this->left->print_Tree();
  }
  if (this->right) {
    this->right->print_Tree();
  }
}

/*
Builds a Decision Tree recursively following a splitting criteria
and stopping at the given depth
Inputs  : int
Outputs :
*/
void DecisionTree::build_Splitted_Tree(int depth) {

  float predicted_Value = this->curr_Node->compute_Predicted_Value();

  if (depth > 0 && (curr_Node->node_Homogeneity() == false) &&
      (predicted_Value >= 0)) {

    if (this->parent) {
      if (predicted_Value ==
          this->get_Parent_Tree()->get_Current_Node()->get_Predicted_Value()) {
        return;
      }
    }
    this->curr_Node->set_Predicted_Value(predicted_Value);
    this->split_Operator->set_Node(this->curr_Node);

    // Set the infos in the Current Node for parsing test dataset
    int split_Feature = this->split_Operator->find_Best_Split_Feature();
    float split_Criteria = this->split_Operator->get_Best_Split_Criteria();

    this->curr_Node->set_Split_Column(split_Feature);
    this->curr_Node->set_Split_Criteria(split_Criteria);

    // Computing the Indexes of the Dataset for each childs
    std::vector<std::vector<int>> child_Indexes =
        this->curr_Node->node_Split(split_Feature, split_Criteria);

    // Creating a left child
    if (child_Indexes[0].empty() == false) {
      auto left_Node = std::make_shared<TreeNode>(
          this->curr_Node->get_DataSet(), child_Indexes[0]);

      auto left_Child =
          std::make_unique<DecisionTree>(left_Node, child_Indexes[0]);

      this->add_Left(std::move(left_Child));

      // Recursive part
      this->left->build_Splitted_Tree(depth - 1);
    }

    // Creating a right child
    if (child_Indexes[1].empty() == false) {
      auto right_Node = std::make_shared<TreeNode>(
          this->curr_Node->get_DataSet(), child_Indexes[1]);

      auto right_Child =
          std::make_unique<DecisionTree>(right_Node, child_Indexes[1]);
      this->add_Right(std::move(right_Child));

      // Recursive part
      this->right->build_Splitted_Tree(depth - 1);
    }
  }
}

/*
Sets a test DataSet in the tree to be parsed after we already
fitted on a DataSet
Inputs  : shared_ptr<DataSet>
Outputs :
*/
void DecisionTree::set_Test_DataSet(std::shared_ptr<DataSet> data) {
  this->curr_Node->set_DataSet(data);

  // Make a vector which size correspond to the number of samples we have
  int len = this->curr_Node->get_Index().size();

  std::vector<float> predicted_Labels(len, 0);
  this->add_Predicted_Labels(
      std::make_shared<std::vector<float>>(predicted_Labels));
}

/*
Parse the DataSet in the tree so each element is classified
in the right Node
Inputs  :
Outputs :
*/
void DecisionTree::parse_Test_DataSet() {

  int position = this->curr_Node->get_Split_Column();
  float criteria = this->curr_Node->get_Split_Criteria();

  std::vector<std::vector<int>> child_Indexes =
      this->curr_Node->node_Split(position, criteria);

  if (this->left) {
    this->left->curr_Node->set_Index(child_Indexes[0]);
    this->left->add_Predicted_Labels(this->get_Predicted_Labels());
    this->left->parse_Test_DataSet();
  }

  if (this->right) {
    this->right->curr_Node->set_Index(child_Indexes[1]);
    this->right->add_Predicted_Labels(this->get_Predicted_Labels());
    this->right->parse_Test_DataSet();
  }
}

/*
Computes the values predictions for the test DataSet
and store them in the predicted_Label parameter
Inputs  :
Outputs :
*/
void DecisionTree::predict_Test_Labels() {

  int size = this->get_Predicted_Labels()->size();

  if (this->curr_Node->get_Predicted_Value() >= 0) {
    for (auto idx : this->curr_Node->get_Index()) {
      if (idx < size) {
        this->predicted_Labels->at(idx) =
            this->curr_Node->get_Predicted_Value();
      }
    }
  }
  if (this->get_Left_Tree()) {
    this->left->predict_Test_Labels();
  }
  if (this->get_Right_Tree()) {
    this->right->predict_Test_Labels();
  }
}

/*
Sums the predicted_Lables value of the given tree
into the object
Inputs  : DecisionTree *
Outputs :
*/
void DecisionTree::sum_Predicted_Labels(DecisionTree *dt) {
  float dt_Prediction = 0;
  float curr_Prediction = this->get_Current_Node()->get_Predicted_Value();

  if (dt) {
    dt_Prediction = dt->get_Current_Node()->get_Predicted_Value();
  }

  this->curr_Node->set_Predicted_Value(dt_Prediction + curr_Prediction);

  if (this->left) {
    if (dt->left) {
      this->left->sum_Predicted_Labels(dt->get_Left_Tree());
    }
  }

  if (this->right) {
    if (dt->right) {
      this->right->sum_Predicted_Labels(dt->get_Right_Tree());
    }
  }
}

/*
Divide every predicted Values stored in the nodes by n
Inputs  : int
Outputs :
*/
void DecisionTree::divide_Predicted_Labels(int n) {
  if (n <= 0) {
    return;
  }
  float curr_Prediction = this->get_Current_Node()->get_Predicted_Value();
  this->curr_Node->set_Predicted_Value(curr_Prediction / n);

  if (this->left) {
    this->left->divide_Predicted_Labels(n);
  }

  if (this->right) {
    this->right->divide_Predicted_Labels(n);
  }
}