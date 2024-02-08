#include <cmath>
#include <omp.h>

#include "MAPE.hpp"
#include "TreeNode.hpp"

/********************/
/*                  */
/*    MAPE CLASS    */
/*                  */
/********************/

/*
Constructor
Parameters :
Inputs     :
Outputs    :
*/
MAPE::MAPE() {}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
MAPE::~MAPE() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void MAPE::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

/*
Return the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
std::string MAPE::get_Name() { return "Mean Absolute Percentage Value"; }

/*
Computes the Mean Absolute Percentage Error of a split on a given column
Index is used to get the column of the dataset that can be accessed
Parameters : position, DataSet, index
Inputs     : int, DataSet, vector<int>
Outputs    : float
*/
float MAPE::compute(int position, const DataSet &data, std::vector<int> index,
                    const float split_Criteria) const {

  float left_MAPE = 0;
  float right_MAPE = 0;

  // Computes the DataSet Row Indexes that child nodes can access
  auto [left_index, right_index] = data.split(position, split_Criteria, index);

  float base_Population = index.size();

  // Creating a left child
  TreeNode left_Child{};

  // Creating a right child
  TreeNode right_Child{};

  // Get the labels
  std::vector<float> labels = data.get_Labels();

  // Computes the Mean Absolute Percentage Error for left child
  float left_Prediction = data.labels_Mean(left_index.value());
  float left_Population = left_index.value().size();

#pragma omp parallel for reduction(+ : left_MAPE)
  for (int idx : left_index.value()) {
    left_MAPE += (std::abs(labels[idx] - left_Prediction)) / left_Prediction;
  }
  left_MAPE *= 100;
  left_MAPE /= left_Population;

  // Computes the Mean Absolute Percentage Error for left child
  float right_Prediction = data.labels_Mean(right_index.value());
  float right_Population = right_index.value().size();

#pragma omp parallel for reduction(+ : right_MAPE)
  for (int idx : right_index.value()) {
    right_MAPE += (std::abs(labels[idx] - right_Prediction)) / right_Prediction;
  }
  right_MAPE *= 100;
  right_MAPE /= right_Population;

  // Compute the result of MAPE for the split at position
  float res =
      ((left_MAPE * left_Population) + (right_MAPE * right_Population)) /
      base_Population;
  return res;
}

/*
Computes the MAPE of two vectors
Parameters : exact results, prediction results
Inputs     : const vector<float>, const vector<float>
Outputs    : double
*/
float MAPE::apply(const std::vector<float> &exact,
                  const std::vector<float> &prediction) {

  float res = 0;
  int size = prediction.size();
#pragma omp parallel for reduction(+ : res)
  for (int i = 0; i < size; ++i) {
    res += std::abs(exact[i] - prediction[i]) / exact[i];
  }

  // Compute the MAPE
  res = (res * 100) / size;

  return res;
}
