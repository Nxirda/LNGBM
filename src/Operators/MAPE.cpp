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
Outputs    : double
*/
double MAPE::compute(int position, const DataSet &data,
                     const std::vector<int> &index,
                     const double split_Criteria) const {

  double left_MAPE = 0.0;
  double right_MAPE = 0.0;

  // Computes the DataSet Row Indexes that child nodes can access
  auto [left_index, right_index] = data.split(position, split_Criteria, index);

  double base_Population = index.size();

  // Creating a left child
  TreeNode left_Child{};

  // Creating a right child
  TreeNode right_Child{};

  // Get the labels
  //std::vector<double> labels = data.get_Labels();

  // Computes the Mean Absolute Percentage Error for left child
  double left_Prediction = data.labels_Mean(left_index.value());
  double left_Population = left_index.value().size();

  // Computes the Mean Absolute Percentage Error for left child
  double right_Prediction = data.labels_Mean(right_index.value());
  double right_Population = right_index.value().size();

//#pragma omp parallel for reduction(+ : left_MAPE)
  for (int idx : left_index.value()) {
    left_MAPE += (std::abs(data.get_Labels()[idx] - left_Prediction)) / left_Prediction;
  }
  left_MAPE *= 100.0;
  left_MAPE *= (1.0 / left_Population);

//#pragma omp parallel for reduction(+ : right_MAPE)
  for (int idx : right_index.value()) {
    right_MAPE += (std::abs(data.get_Labels()[idx] - right_Prediction)) / right_Prediction;
  }
  right_MAPE *= 100.0;
  right_MAPE *= (1.0 / right_Population);

  // Compute the result of MAPE for the split at position
  double res =
      ((left_MAPE * left_Population) + (right_MAPE * right_Population)) /
      base_Population;
  return res;
}

/*
Computes the MAPE of two vectors
Parameters : exact results, prediction results
Inputs     : const vector<double>, const vector<double>
Outputs    : double
*/
double MAPE::apply(const std::vector<double> &exact,
                   const std::vector<double> &prediction) {

  double res = 0.0;
  int size = prediction.size();
//#pragma omp parallel for reduction(+ : res)
  for (int i = 0; i < size; ++i) {
    res += std::abs(exact[i] - prediction[i]) * (1.0 / exact[i]);
  }

  // Compute the MAPE
  res = (res * 100.0) * (1.0 / (double)size);

  return res;
}
