#include <cmath>

#include "MAPE.hpp"
#include "TreeNode.hpp"

/********************/
/*                  */
/*    MAPE CLASS    */
/*                  */
/********************/

/*
Constructor
Inputs  : DecisionTree*
Outputs :
*/
MAPE::MAPE() {}

/*
Destructor
Inputs  :
Outputs :
*/
MAPE::~MAPE() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Inputs  :
Outputs :
*/
void MAPE::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

/*
Computes the Mean Absolute Percentage Error of a split on a given column
Inputs  : int
Outputs : float
*/
float MAPE::compute(int position, const DataSet &data,
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

  // Get the labels
  std::vector<float> labels = data.get_Labels(index);

  // Computes the Mean Absolute Percentage Error for left child
  float left_Prediction = data.labels_Mean(*left_index);
  float left_MAPE = 0;
  for (int idx : *left_index) {
    left_MAPE += (abs(labels[idx] - left_Prediction)) / left_Prediction;
  }

  // Computes the Mean Absolute Percentage Error for left child
  float right_Prediction = data.labels_Mean(*right_index);
  float right_MAPE = 0;
  for (int idx : *right_index) {
    right_MAPE += (abs(labels[idx] - right_Prediction)) / right_Prediction;
  }

  left_MAPE *= 100;
  right_MAPE *= 100;

  // Compute the result of MAE for the split at position
  float res = (left_MAPE + right_MAPE) / base_Population;
  return res;
}