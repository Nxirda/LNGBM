#include <cmath>
#include <omp.h>

#include "RMSE.hpp"
#include "TreeNode.hpp"

/********************/
/*                  */
/*    RMSE CLASS    */
/*                  */
/********************/

/*
Constructor
Parameters :
Input      :
Output     :
*/
RMSE::RMSE() {}

/*
Destructor
Parameters :
Input      :
Output     :
*/
RMSE::~RMSE() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Parameters :
Input      :
Output     :
*/
void RMSE::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

/*
Return the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
std::string RMSE::get_Name() { return "Root Mean Square Error"; }

/*
Computes the Root Mean Square Error of a split on a given column
Index is used to get the column of the dataset that can be accessed
Parameters : position, DataSet, index
Inputs     : int, DataSet, vector<int>
Outputs    : double
*/
double RMSE::compute(int position, const DataSet &data,
                     const std::vector<int> &index,
                     const double split_Criteria) const {

  double right_RMSE = 0.0;
  double left_RMSE = 0.0;

  // Computes the DataSet Row Indexes that child nodes can access
  auto [left_index, right_index] = data.split(position, split_Criteria, index);

  double base_Population = index.size();

  // Creating a left child
  TreeNode left_Child{};

  // Creating a right child
  TreeNode right_Child{};

  // Get the labels
  std::vector<double> labels = data.get_Labels();

  // Computes the Root Mean Square Error for left child
  double left_Prediction = data.labels_Mean(left_index.value());
  double left_Population = left_index.value().size();

#pragma omp parallel for reduction(+ : left_RMSE)
  for (int idx : left_index.value()) {
    left_RMSE += pow((std::abs(labels[idx] - left_Prediction)), 2) *
                 (1.0 / left_Population);
    ;
  }
  left_RMSE = sqrt(left_RMSE);

  // Computes the Root Mean Square Error for left child
  double right_Prediction = data.labels_Mean(right_index.value());
  double right_Population = right_index.value().size();

#pragma omp parallel for reduction(+ : right_RMSE)
  for (int idx : right_index.value()) {
    right_RMSE += pow((std::abs(labels[idx] - right_Prediction)), 2) *
                  (1.0 / right_Population);
  }
  right_RMSE = sqrt(right_RMSE);

  // Compute the result of RMSE for the split at position
  double res =
      ((left_RMSE * left_Population) + (right_RMSE * right_Population)) *
      (1.0 / base_Population);
  return res;
}