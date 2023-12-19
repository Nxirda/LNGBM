#include <iostream>

#include "ReductionInVar.hpp"
#include "TreeNode.hpp"

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
ReductionInVar::ReductionInVar() {}

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
Returns the variance of a split as the weighted average
variance of child nodes
Inputs  : int
Outputs : float
*/
float ReductionInVar::compute(int position, const DataSet &data,
                              std::vector<int> index) const {

  // Computes the split criteria, needs to be not hardcoded in the future
  float split_Criteria = data.column_Mean(position, index);

  // Computes the DataSet Row Indexes that child nodes can access
  auto [left_index, right_index] = data.split(position, split_Criteria, index);

  float base_Population = index.size();

  // Creating a left child
  TreeNode left_Child{};

  // Creating a right child
  TreeNode right_Child{};

  // Computes Weighted Variance for left child
  float left_Variance = data.labels_Variance(*left_index);
  float left_Weighted_Average = left_index.value().size() / base_Population;

  // Computes Weighted Variance for right child
  float right_Variance = data.labels_Variance(*right_index);
  float right_Weighted_Average = right_index.value().size() / base_Population;

  // Computes weighted Average Variance for the two Nodes
  float weighted_Average_Var = (left_Weighted_Average * left_Variance +
                                right_Weighted_Average * right_Variance);

  return weighted_Average_Var;
}