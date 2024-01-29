#include <cmath>
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
Parameters :
Inputs     :
Outputs    :
*/
ReductionInVar::ReductionInVar() {}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
ReductionInVar::~ReductionInVar() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void ReductionInVar::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

/*
Computes the Variance of a split on a given column
Index is used to get the column of the dataset that can be accessed
Parameters : position, DataSet, index
Inputs     : int, DataSet, vector<int>
Outputs    : float
*/
float ReductionInVar::compute(int position, const DataSet &data,
                              std::vector<int> index,
                              const float split_Criteria) const {

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

/*
Computes the Variance of two vectors
Parameters : exact results, prediction results
Inputs     : const vector<float>, const vector<float>
Outputs    : double
*/
double ReductionInVar::apply(const std::vector<float> &exact,
                             const std::vector<float> &prediction) {
  double res = 0;
  float size = prediction.size();

  for (unsigned long int i = 0; i < size; ++i) {
    res += std::pow((exact[i] - prediction[i]), 2);
  }

  // Compute the Variance
  res = res / size;
  return res;
}