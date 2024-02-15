#include <cmath>
#include <omp.h>

#include "RIV.hpp"
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
RIV::RIV() {}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
RIV::~RIV() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void RIV::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

/*
Return the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
std::string RIV::get_Name() { return "Reduction In Variance"; }

/*
Computes the Variance of a split on a given column
Index is used to get the column of the dataset that can be accessed
Parameters : position, DataSet, index
Inputs     : int, DataSet, vector<int>
Outputs    : double
*/
double RIV::compute(int position, const DataSet &data,
                    const std::vector<int> &index,
                    const double split_Criteria) const {

  // Computes the DataSet Row Indexes that child nodes can access
  auto [left_index, right_index] = data.split(position, split_Criteria, index);

  double base_Population = index.size();

  // Creating a left child
  TreeNode left_Child{};

  // Creating a right child
  TreeNode right_Child{};

  // Computes Weighted Variance for left child
  double left_Variance = data.labels_Variance(*left_index);
  double left_Weighted_Average =
      left_index.value().size() * (1.0 / base_Population);

  // Computes Weighted Variance for right child
  double right_Variance = data.labels_Variance(*right_index);
  double right_Weighted_Average =
      right_index.value().size() * (1.0 / base_Population);

  // Computes weighted Average Variance for the two Nodes
  double weighted_Average_Var = (left_Weighted_Average * left_Variance +
                                 right_Weighted_Average * right_Variance);

  return weighted_Average_Var;
}

/*
Computes the Variance of two vectors
Parameters : exact results, prediction results
Inputs     : const vector<double>, const vector<double>
Outputs    : double
*/
double RIV::apply(const std::vector<double> &exact,
                  const std::vector<double> &prediction) {
  double res = 0.0;
  int size = prediction.size();

#pragma omp parallel for reduction(+ : res)
  for (int i = 0; i < size; ++i) {
    res += std::pow((exact[i] - prediction[i]), 2);
  }

  // Compute the Variance
  res *= (1.0 / size);
  return res;
}