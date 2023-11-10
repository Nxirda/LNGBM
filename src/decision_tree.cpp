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

/* Constructor with Dataset :                       */
/* Inputs  : Object  of Dataset Class               */
/* Outputs : TreeNode Object containing the Dataset */
TreeNode::TreeNode(const Dataset &d) { this->data = d; }

/* Override "=" operator                 */
/* Inputs  : Object of TreeNode Class    */
/* Outputs : Object of TreeNode Class    */
TreeNode &TreeNode::operator=(TreeNode const &TN) {
  data = get_Dataset();
  return *this;
}

/* Returns the Node's Dataset        */
/* Inputs  :                         */
/* Outputs : Object of Dataset Class */
Dataset TreeNode::get_Dataset() { return this->data; }

/* Returns the Variance of the Node's Dataset */
/* Inputs  :                                  */
/* Outputs : float                            */
float TreeNode::NodeVariance() { return this->get_Dataset().Global_Variance(); }

/* return the Homogeneity as a boolean by comparing the variance */
/* for every Column of the given Tree Node Dataset               */
/* Inputs  :                                                     */
/* Outputs : bool                                                */
bool TreeNode::NodeHomogeneity() {
  int len = this->get_Dataset().Label_length();
  float var = this->get_Dataset().Column_Variance(0);
  for (int i = 1; i < len; ++i) {
    float tmp = this->get_Dataset().Column_Variance(i);
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

/* Base Constructor with a Dataset parameter */
/* Inputs  : Object of Dataset Class         */
/* Outputs : Object of Decision Tree Class   */
DecisionTree::DecisionTree(const Dataset &data) {
  this->Parent = nullptr;
  this->Curr_Node = std::move(new TreeNode{data});
  this->Left = nullptr;
  this->Right = nullptr;
}

/* Override "=" operator                   */
/* Inputs  : Object of Decision Tree Class */
/* Outputs : Object of Decision Tree Class */
DecisionTree &DecisionTree::operator=(const DecisionTree &DT) {
  Parent = &get_ParentTree();
  Curr_Node = &get_Current_Node();
  Left = &get_LeftTree();
  Right = &get_RightTree();

  return *this;
}

/* Default Destructor */
/* Inputs  :          */
/* Outputs :          */
DecisionTree::~DecisionTree() {
  delete this->Curr_Node;
  delete this->Left;
  delete this->Right;
}

/* Returns the Current Node of the Tree      */
/* Inputs  :                                 */
/* Outputs : pointer of Decision Tree Object */
TreeNode &DecisionTree::get_Current_Node() { return *this->Curr_Node; }

/* Returns the Parent Tree                   */
/* Inputs  :                                 */
/* Outputs : pointer of Decision Tree Object */
DecisionTree &DecisionTree::get_ParentTree() { return *this->Parent; }

/* Returns the Left Sub Tree                 */
/* Inputs  :                                 */
/* Outputs : pointer of Decision Tree Object */
DecisionTree &DecisionTree::get_LeftTree() { return *this->Left; }

/* Returns the Right Sub Tree                */
/* Inputs  :                                 */
/* Outputs : pointer of Decision Tree Object */
DecisionTree &DecisionTree::get_RightTree() { return *this->Right; }

/* Sets a new Parent for the given tree      */
/* Inputs  : pointer of Decision Tree Object */
/* Outputs :                                 */
void DecisionTree::add_Parent(DecisionTree *d) { this->Parent = std::move(d); }

/* Sets a new left Subtree          */
/* Inputs : Object of Dataset class */
/* Output :                         */
void DecisionTree::add_left(Dataset data) {
  this->Left = std::move(new DecisionTree{data});
  this->Left->add_Parent(this);
}

/* Sets a new right Subtree         */
/* Inputs : Object of Dataset class */
/* Output :                         */
void DecisionTree::add_right(Dataset data) {
  this->Right = std::move(new DecisionTree{data});
  this->Right->add_Parent(this);
}

/* Print function for Decision Trees */
/* Inputs  :                         */
/* Outputs :                         */
void DecisionTree::print_Tree() {
  this->get_Current_Node().get_Dataset().print();
  if (&this->get_LeftTree() != nullptr) {
    this->get_LeftTree().print_Tree();
  }
  if (&this->get_RightTree() != nullptr) {
    this->get_RightTree().print_Tree();
  }
}

/* Returns the variance of a split as the weighted average */
/* variance of child nodes                                 */
/* Inputs  : int                                           */
/* Outputs : float                                         */
float DecisionTree::splitting_variance(int position) {
  float split_criteria =
      this->get_Current_Node().get_Dataset().Column_Mean(position);
  vector<Dataset> Child_Nodes =
      this->get_Current_Node().get_Dataset().split(position, split_criteria);

  float base_Population = this->get_Current_Node().get_Dataset().Entries_size();

  float Left_Variance = Child_Nodes[0].Global_Variance();
  float Left_weighted_average = Child_Nodes[0].Entries_size() / base_Population;
  float Right_Variance = Child_Nodes[1].Global_Variance();
  float Right_weighted_average =
      Child_Nodes[1].Entries_size() / base_Population;

  float weighted_average_var = Left_weighted_average * Left_Variance +
                               Right_weighted_average * Right_Variance;

  return weighted_average_var;
}

/* Search for the best attribute to split the dataset on at a given Node */
/* Inputs :                                                              */
/* Ouputs : String                                                       */
std::string DecisionTree::FindBestAttribute() {
  std::string BestAttribute = "";
  float maxReductionInVar = INT_MAX;

  vector<string> labels = this->get_Current_Node().get_Dataset().get_Labels();

  for (int i = 0; i < labels.size(); ++i) {
    float tmp_var = splitting_variance(i);
    if (tmp_var < maxReductionInVar) {
      maxReductionInVar = tmp_var;
      BestAttribute = labels[i];
    }
  }
  return BestAttribute;
}

/* Builds a Decision Tree recursively following a splitting criteria */
/* Inputs  :                                                         */
/* Outputs :                                                         */
void DecisionTree::Build_Splitted_Tree(DecisionTree *DT) {}
