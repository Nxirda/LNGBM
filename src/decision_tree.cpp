#include <fstream>
#include <iostream>
#include <iterator>
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

/********************/
/*                  */
/*    TREE PART     */
/*                  */
/********************/

/* Base Constructor with a Dataset parameter : */
/* Inputs  : Object of Dataset Class           */
/* Outputs : Object of Decision Tree Class     */
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

float Variance(vector<float> Current_Column) {
  int len = Current_Column.size();
  // check if there are values in the current node
  if (len <= 0) {
    return 0.0;
  }
  float mean = 0;
  float variance = 0.0;
  for (float elem : Current_Column) {
    mean += elem;

    float difference = elem - len;
    variance += difference * difference;
  }
  mean /= len;
  variance /= len;

  return variance;
}

float ReductionInVariance(DecisionTree *DT, std::string label, int position) {
  float CurrNode_Var = Variance(DT);
  float Var = 0;
  vector<float> values =
      DT->get_Current_Node().get_Dataset().get_Column(position);
  float Dataset_Length = DT->get_Current_Node().get_Dataset().Label_length();

  for (float value : values) {
    Var += (values.size() / Dataset_Length) * Variance(values);
  }

  return CurrNode_Var - Var;
}

void Build_Splitted_Tree(DecisionTree *DT) {}

/* Search for the best attribute to split the dataset on at a given Node */
/* Inputs : Object of DecisionTree class                                 */
/* Ouputs : String                                                       */
std::string FindBestAttribute(DecisionTree *DT) {
  std::string BestAttribute = "";
  float reductionInVar = -1;

  vector<string> labels = DT->get_Current_Node().get_Dataset().get_Labels();

  for (int i = 0; i < labels.size(); ++i) {
    float tmp_var = ReductionInVariance(DT, labels[i], i);
    if (tmp_var > reductionInVar) {
      reductionInVar = tmp_var;
      BestAttribute = labels[i];
    }
  }
  return BestAttribute;
}

