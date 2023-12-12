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
float ReductionInVar::splitting_RIV(int position, const DataSet &data,
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
  float left_Variance = data.column_Variance(left_index);
  // float left_Variance = left_Child.node_Variance();
  float left_Weighted_Average = left_index.size() / base_Population;

  // Computes Weighted Variance for right child
  float right_Variance = data.column_Variance(right_index);
  // float right_Variance = right_Child.node_Variance();
  float right_Weighted_Average = right_index.size() / base_Population;

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
std::tuple<int, float>
ReductionInVar::find_Best_Split(const DataSet &data,
                                        std::vector<int> index) const {
  int best_Feature = 0;
  float max_Reduction_In_Var = INT_MAX;

  std::vector<std::string> features =
      data.get_Features();

  for (unsigned long int i = 0; i < features.size(); ++i) {
    float tmp_var = splitting_RIV(i, data, index);
    if (tmp_var < max_Reduction_In_Var) {
      max_Reduction_In_Var = tmp_var;
      best_Feature = i;
    }
  }
  //this->set_Split_Criteria(this->tree_Node->node_Column_Mean(best_Feature));
  float criterion = data.column_Mean(best_Feature, index);
  return std::make_tuple(best_Feature, criterion);
}