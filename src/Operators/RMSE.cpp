#include <cmath>

#include "RMSE.hpp"
#include "TreeNode.hpp"

/********************/
/*                  */
/*    RMSE CLASS    */
/*                  */
/********************/

/*
Constructor
Input  : DecisionTree*
Output :
*/
RMSE::RMSE() {}

/*
Destructor
Input  :
Output :
*/
RMSE::~RMSE() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Input  :
Output :
*/
void RMSE::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

/*
Computes the Root Mean Square Error of a split on a given column
Inputs  : int
Outputs : float
*/
float RMSE::compute(int position, const DataSet &data,
                    std::vector<int> index) const {

  float right_RMSE = 0;
  float left_RMSE = 0;

  // Computes the split criteria, needs to be not hardcoded in the future
  float split_Criteria = data.column_Mean(position, index);

  // Computes the DataSet Row Indexes that child nodes can access
  auto [left_index, right_index] = data.split(position, split_Criteria, index);

  float base_Population = index.size();

  // Creating a left child
  TreeNode left_Child{};

  // Creating a right child
  TreeNode right_Child{};

  // Get the labels
  std::vector<float> labels = data.get_Labels();

  // Computes the Mean Absolute Error for left child
  float left_Prediction = data.labels_Mean(left_index.value());
  float left_Population = left_index.value().size();
  for (int idx : left_index.value()) {
    left_RMSE += pow((std::abs(labels[idx] - left_Prediction)), 2);
  }
  left_RMSE /= left_Population;
  left_RMSE = sqrt(left_RMSE);

  // Computes the Mean Absolute Error for left child
  float right_Prediction = data.labels_Mean(right_index.value());
  float right_Population = left_index.value().size();
  for (int idx : right_index.value()) {
    right_RMSE += pow((std::abs(labels[idx] - right_Prediction)), 2);
  }
  right_RMSE /= right_Population;
  right_RMSE = sqrt(right_RMSE);

  // Compute the result of MAE for the split at position
  float res =
      ((left_RMSE * left_Population) + (right_RMSE * right_Population)) /
      base_Population;
  return res;
}