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
/*    TREE PART     */
/*                  */
/********************/

/* Default Constructor                     */
/* Inputs  :                               */
/* Outputs : Object of Decision Tree Class */
DecisionTree::DecisionTree() {}

/* Base Constructor with a DecisionTree parameter */
/* Inputs  : Object of DecisionTree Class         */
/* Outputs : Object of Decision Tree Class        */
DecisionTree::DecisionTree(const DecisionTree &dt) {
  parent = dt.parent;
  curr_Node = dt.curr_Node;
  left = std::make_unique<DecisionTree>(get_Left_Tree());
  right = std::make_unique<DecisionTree>(get_Right_Tree());
}

/* Base Constructor with a DataSet parameter */
/* Inputs  : Object of DataSet Class         */
/* Outputs : Object of Decision Tree Class   */
DecisionTree::DecisionTree(const DataSet &data) {
  std::vector<int> idx;
  for (int i = 0; i < data.samples_Number(); ++i) {
    idx.push_back(i);
  }
  std::shared_ptr<DataSet> dataset = std::make_shared<DataSet>(data);
  this->parent = nullptr;
  this->curr_Node =
      std::make_shared<TreeNode>(TreeNode{dataset, idx}); 
  this->left = nullptr;
  this->right = nullptr;
}

/* Override "=" operator                   */
/* Inputs  : Object of Decision Tree Class */
/* Outputs : Object of Decision Tree Class */
DecisionTree &DecisionTree::operator=(DecisionTree &dt) {
  parent = nullptr; // std::move(dt.get_Parent_Tree()); // To be corrected
  curr_Node = dt.curr_Node;
  left = nullptr;  // std::make_unique<DecisionTree>(dt.get_Left_Tree());
  right = nullptr; // std::make_unique<DecisionTree>(dt.get_Right_Tree());

  return *this;
}

/* Default Destructor */
/* Inputs  :          */
/* Outputs :          */
DecisionTree::~DecisionTree(){}; // delete this->curr_Node; }

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
void DecisionTree::add_Parent(DecisionTree *d) { this->parent = d; }

/* Sets a new left Subtree          */
/* Inputs : Object of DataSet class */
/* Output :                         */
void DecisionTree::add_Left(std::unique_ptr<DecisionTree> dt) {
  this->left = std::move(dt);
  this->left->add_Parent(this);
}

/* Sets a new right Subtree         */
/* Inputs : Object of DataSet class */
/* Output :                         */
void DecisionTree::add_Right(std::unique_ptr<DecisionTree> dt) {
  this->right = std::move(dt);
  this->right->add_Parent(this);
}

/* Print function for Decision Trees */
/* Inputs  :                         */
/* Outputs :                         */
void DecisionTree::print_Tree() {
  this->get_Current_Node().get_DataSet().print();
  if (this->left) {
    this->get_Left_Tree().print_Tree();
  }
  if (this->right) {
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
std::string DecisionTree::find_Best_Feature() {
  std::string best_Feature = "";
  float max_Reduction_In_Var = INT_MAX;

  std::vector<std::string> features =
      this->get_Current_Node().get_DataSet().get_Features();

  for (unsigned long int i = 0; i < features.size(); ++i) {
    float tmp_var = splitting_Variance(i);
    if (tmp_var < max_Reduction_In_Var) {
      max_Reduction_In_Var = tmp_var;
      best_Feature = features[i];
    }
  }
  return best_Feature;
}

/* Builds a Decision Tree recursively following a splitting criteria */
/* Inputs  :                                                         */
/* Outputs :                                                         */
// void DecisionTree::build_Splitted_Tree(DecisionTree *dt) {}
