#include <fstream>
#include <iostream>
#include <iterator>
#include <limits.h> //For Max_Int value
#include <string>
#include <vector>

#include "../include/decision_tree.h"

/********************/
/*                  */
/*    NODE PART     */
/*                  */
/********************/

/* Default Constructor                */
/* Inputs  :                          */
/* Outputs : Object of TreeNode Class */
TreeNode::TreeNode() {}

/* Default Destructor          */
/* Inputs  :                   */
/* Outputs :                   */
TreeNode::~TreeNode() {}

/* Constructor with DataSet :                       */
/* Inputs  : Object  of DataSet Class               */
/* Outputs : TreeNode Object containing the DataSet */
TreeNode::TreeNode(const DataSet &d) { this->data = d; }

/* Override "=" operator                 */
/* Inputs  : Object of TreeNode Class    */
/* Outputs : Object of TreeNode Class    */
TreeNode &TreeNode::operator=(TreeNode const &tn) {
  data = get_DataSet();
  return *this;
}

/* Returns the Node's DataSet        */
/* Inputs  :                         */
/* Outputs : Object of DataSet Class */
DataSet TreeNode::get_DataSet() { return this->data; }

/* Returns the Variance of the Node's DataSet */
/* Inputs  :                                  */
/* Outputs : float                            */
float TreeNode::node_Variance() {
  return this->get_DataSet().global_Variance();
}

/* return the Homogeneity as a boolean by comparing the variance */
/* for every Column of the given Tree Node DataSet               */
/* Inputs  :                                                     */
/* Outputs : bool                                                */
bool TreeNode::node_Homogeneity() {
  int len = this->get_DataSet().features_Length();
  float var = this->get_DataSet().column_Variance(0);
  for (int i = 1; i < len; ++i) {
    float tmp = this->get_DataSet().column_Variance(i);
    //  If column variance is not uniform then return false
    if (tmp != var) {
      return false;
    }
  }
  return true;
}

/********************/
/*                  */
/*    TREE PART     */
/*                  */
/********************/

/* Base Constructor with a DataSet parameter */
/* Inputs  : Object of DataSet Class         */
/* Outputs : Object of Decision Tree Class   */
DecisionTree::DecisionTree(const DataSet &data) {
  this->parent = nullptr;
  this->curr_Node = std::move(new TreeNode{data});
  this->left = nullptr;
  this->right = nullptr;
}

/* Override "=" operator                   */
/* Inputs  : Object of Decision Tree Class */
/* Outputs : Object of Decision Tree Class */
DecisionTree &DecisionTree::operator=(const DecisionTree &dt) {
  parent = &get_Parent_Tree();
  curr_Node = &get_Current_Node();
  left = &get_Left_Tree();
  right = &get_Right_Tree();

  return *this;
}

/* Default Destructor */
/* Inputs  :          */
/* Outputs :          */
DecisionTree::~DecisionTree() {
  delete this->curr_Node;
  delete this->left;
  delete this->right;
}

/* Returns the Current Node of the Tree      */
/* Inputs  :                                 */
/* Outputs : pointer of Decision Tree Object */
TreeNode &DecisionTree::get_Current_Node() { return *this->curr_Node; }

/* Returns the Parent Tree                   */
/* Inputs  :                                 */
/* Outputs : pointer of Decision Tree Object */
DecisionTree &DecisionTree::get_Parent_Tree() { return *this->parent; }

/* Returns the Left Sub Tree                 */
/* Inputs  :                                 */
/* Outputs : pointer of Decision Tree Object */
DecisionTree &DecisionTree::get_Left_Tree() { return *this->left; }

/* Returns the Right Sub Tree                */
/* Inputs  :                                 */
/* Outputs : pointer of Decision Tree Object */
DecisionTree &DecisionTree::get_Right_Tree() { return *this->right; }

/* Sets a new Parent for the given tree      */
/* Inputs  : pointer of Decision Tree Object */
/* Outputs :                                 */
void DecisionTree::add_Parent(DecisionTree *d) { this->parent = std::move(d); }

/* Sets a new left Subtree          */
/* Inputs : Object of DataSet class */
/* Output :                         */
void DecisionTree::add_Left(DataSet data) {
  this->left = std::move(new DecisionTree{data});
  this->left->add_Parent(this);
}

/* Sets a new right Subtree         */
/* Inputs : Object of DataSet class */
/* Output :                         */
void DecisionTree::add_Right(DataSet data) {
  this->right = std::move(new DecisionTree{data});
  this->right->add_Parent(this);
}

/* Print function for Decision Trees */
/* Inputs  :                         */
/* Outputs :                         */
void DecisionTree::print_Tree() {
  this->get_Current_Node().get_DataSet().print();
  if (&this->get_Left_Tree() != nullptr) {
    this->get_Left_Tree().print_Tree();
  }
  if (&this->get_Right_Tree() != nullptr) {
    this->get_Right_Tree().print_Tree();
  }
}

/* Returns the variance of a split as the weighted average */
/* variance of child nodes                                 */
/* Inputs  : int                                           */
/* Outputs : float                                         */
float DecisionTree::splitting_Variance(int position) {
  float split_Criteria =
      this->get_Current_Node().get_DataSet().column_Mean(position);

  std::vector<DataSet> child_Nodes =
      this->get_Current_Node().get_DataSet().split(position, split_Criteria);

  float base_Population =
      this->get_Current_Node().get_DataSet().samples_Number();

  float left_Variance = child_Nodes[0].global_Variance();
  float left_Weighted_Average =
      child_Nodes[0].samples_Number() / base_Population;

  float right_Variance = child_Nodes[1].global_Variance();
  float right_Weighted_Average =
      child_Nodes[1].samples_Number() / base_Population;

  float weighted_Average_Var = left_Weighted_Average * left_Variance +
                               right_Weighted_Average * right_Variance;

  return weighted_Average_Var;
}

/* Search for the best attribute to split the dataset on at a given Node */
/* Inputs :                                                              */
/* Ouputs : String                                                       */
std::string DecisionTree::find_Best_Attribute() {
  std::string best_Attribute = "";
  float max_Reduction_In_Var = INT_MAX;

  std::vector<std::string> labels =
      this->get_Current_Node().get_DataSet().get_Features();

  for (int i = 0; i < labels.size(); ++i) {
    float tmp_var = splitting_Variance(i);
    if (tmp_var < max_Reduction_In_Var) {
      max_Reduction_In_Var = tmp_var;
      best_Attribute = labels[i];
    }
  }
  return best_Attribute;
}

/* Builds a Decision Tree recursively following a splitting criteria */
/* Inputs  :                                                         */
/* Outputs :                                                         */
void DecisionTree::build_Splitted_Tree(DecisionTree *dt) {}
