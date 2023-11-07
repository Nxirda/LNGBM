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

/* Computes the Variance of a given Column from the Tree Dataset */
/* Inputs  : vector<float>                                       */
/* Outputs : float                                               */
float DecisionTree::Variance(vector<float> Current_Column) {
  int len = Current_Column.size();
  // cout << "Len of Column is : " << len << endl;
  //  check if there are values in the current node
  if (len <= 0) {
    return 0.0;
  }
  float mean = 0;
  float variance = 0.0;
  for (float elem : Current_Column) {
    // cout << "Elem of Column is : " << elem << endl;
    mean += elem;
  }
  mean /= len;
  // cout << "Mean of Column is : " << mean << endl;
  for (float elem : Current_Column) {
    float difference = elem - mean;
    variance += difference * difference;
  }
  variance /= len;
  // cout << "Variance of Column is : " << variance << endl;
  return variance;
}

/* Computes the Variance for every Column of the given Tree Dataset */
/* Inputs  : pointer of Decision Tree Object                        */
/* Outputs : float                                                  */
float DecisionTree::NodeHomogeneity(DecisionTree *DT) {
  int len = DT->get_Current_Node().get_Dataset().Label_length();
  float Homogeneous = 0;
  for (int i = 0; i < len; ++i) {
    Homogeneous += Variance(DT->get_Current_Node().get_Dataset().get_Column(i));
  }
  Homogeneous /= len;
  return Homogeneous;
}

/* Computes the Reduction in Variance of a given Column of the Tree Dataset */
/* Inputs  : pointer of Decision Tree Object, string, int                   */
/* Outputs : float                                                          */
float DecisionTree::ReductionInVariance(DecisionTree *DT, std::string label,
                                        int position) {
  float CurrNode_Var = NodeHomogeneity(DT);
  std::cout << "Homogeneity is :" << CurrNode_Var << endl;

  float Var = 0;
  vector<float> values =
      DT->get_Current_Node().get_Dataset().get_Column(position);
  float Dataset_Length = DT->get_Current_Node().get_Dataset().Label_length();

  Var = (values.size() / Dataset_Length) * Variance(values);
  std::cout << "Var is :" << Var << endl;

  std::cout << "ReductionInVariance result is :" << CurrNode_Var - Var << endl;
  return CurrNode_Var - Var;
}

/* Builds a Decision Tree recursively following a splitting criteria */
/* Inputs  :                                                         */
/* Outputs :                                                         */
void DecisionTree::Build_Splitted_Tree(DecisionTree *DT) {}

/* Search for the best attribute to split the dataset on at a given Node */
/* Inputs : Object of DecisionTree class                                 */
/* Ouputs : String                                                       */
std::string DecisionTree::FindBestAttribute() {
  std::string BestAttribute = "";
  float reductionInVar = -1;

  vector<string> labels = this->get_Current_Node().get_Dataset().get_Labels();

  for (int i = 0; i < labels.size(); ++i) {
    float tmp_var = ReductionInVariance(this, labels[i], i);
    if (tmp_var > reductionInVar) {
      reductionInVar = tmp_var;
      BestAttribute = labels[i];
    }
  }
  return BestAttribute;
}
