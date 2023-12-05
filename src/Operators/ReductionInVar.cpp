#include <iostream>

#include "TreeNode.hpp"
#include "ReductionInVar.hpp"

/********************/
/*                  */
/*    RIV CLASS     */
/*                  */
/********************/

/*
Constructor
Inputs  : DecisionTree*
Outputs :
*/
ReductionInVar::ReductionInVar(std::shared_ptr<TreeNode> tree_Node) {this->tree_Node = tree_Node;}

/*
Setter for the tree pointer
Inputs  : DecisionTree*
Outputs : bool
*/
bool ReductionInVar::set_Node(std::shared_ptr<TreeNode> tree_Node) {
  this->tree_Node = tree_Node;
  this->split_Criteria = 0.0;
  if (this->tree_Node) {
    return true;
  }
  return false;
}

/*
Destructor
Inputs  :
Outputs :
*/
ReductionInVar::~ReductionInVar() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Inputs  :
Outputs :
*/
void ReductionInVar::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

/*
Returns the best splitting criteria for RIV algorithm
Inputs  :
Outputs : float
*/
float ReductionInVar::get_Best_Split_Criteria() { return this->split_Criteria; }

/*
Sets the split criteria as the value given
Inputs  : float
Outputs :
*/
void ReductionInVar::set_Split_Criteria(float value) {
  this->split_Criteria = value;
}

/*
Returns the variance of a split as the weighted average
variance of child nodes
Inputs  : int
Outputs : float
*/
float ReductionInVar::splitting_Variance(int position) {
  // Computes the split criteria, needs to be not hardcoded in the future
  float split_Criteria =
      this->tree_Node->node_Column_Mean(position);

  float base_Population = this->tree_Node->get_Index().size();


  // Computes the DataSet Row Indexes that child nodes can access
  std::vector<std::vector<int>> child_Indexes =
      this->tree_Node->node_Split(position, split_Criteria);

  // Creating a left child
  TreeNode left_Child{this->tree_Node->get_DataSet(),
                      child_Indexes[0]};

  // Creating a right child
  TreeNode right_Child{this->tree_Node->get_DataSet(),
                       child_Indexes[1]};

  // Computes Weighted Variance for left child
  float left_Variance = left_Child.node_Variance();
  float left_Weighted_Average = left_Child.get_Index().size() / base_Population;

  // Computes Weighted Variance for right child
  float right_Variance = right_Child.node_Variance();
  float right_Weighted_Average =
      right_Child.get_Index().size() / base_Population;

  // Computes weighted Average Variance for the two Nodes
  float weighted_Average_Var = (left_Weighted_Average * left_Variance +
                                right_Weighted_Average * right_Variance);

  return weighted_Average_Var;
}

/*
Search for the best feature to split the dataset on at a given Node
Inputs :
Ouputs : int
*/
int ReductionInVar::find_Best_Split_Feature() {
  int best_Feature = 0;
  float max_Reduction_In_Var = INT_MAX;

  std::vector<std::string> features =
      this->tree_Node->get_DataSet()->get_Features();

  for (unsigned long int i = 0; i < features.size() ; ++i) {
    float tmp_var = splitting_Variance(i);
    if (tmp_var < max_Reduction_In_Var) {
      max_Reduction_In_Var = tmp_var;
      best_Feature = i;
    }
  }
  this->set_Split_Criteria(
      this->tree_Node->node_Column_Mean(best_Feature));

  return best_Feature;
}