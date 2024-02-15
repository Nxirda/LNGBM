#include <cmath>
#include <omp.h>

#include "MAE.hpp"
#include "TrainingElement.hpp"

/********************/
/*                  */
/*    MAE CLASS     */
/*                  */
/********************/

/*
Constructor
Parameters :
Inputs     :
Outputs    :
*/
MAE::MAE() {}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
MAE::~MAE() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void MAE::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

/*
Return the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
std::string MAE::get_Name() { return "Mean Absolute Value"; }

/*
Computes the Mean Absolute Error of a split on a given column
Index is used to get the column of the dataset that can be accessed
Parameters : position, DataSet, index
Inputs     : int, DataSet, vector<int>
Outputs    : double
*/
double MAE::compute(int position, const DataSet &data,
                    const std::vector<int> &index,
                    const double split_Criteria) const {

  double left_MAE = 0.0;
  double right_MAE = 0.0;

  // Computes the DataSet Row Indexes that child nodes can access
  auto [left_index, right_index] = data.split(position, split_Criteria, index);

  double base_Population = index.size();

  // Creating a left child
  TreeNode left_Child{};

  // Creating a right child
  TreeNode right_Child{};

  // Get the labels
  std::vector<double> labels = data.get_Labels();

  // Computes the Mean Absolute Error for left child
  double left_Prediction = data.labels_Mean(left_index.value());
  double left_Population = left_index.value().size();

  // Computes the Mean Absolute Error for right child
  double right_Prediction = data.labels_Mean(right_index.value());
  double right_Population = right_index.value().size();

#pragma omp parallel for reduction(+ : left_MAE)
  for (int idx : left_index.value()) {
    left_MAE += std::abs(labels[idx] - left_Prediction);
  }
  left_MAE *= (1.0 / left_Population);

#pragma omp parallel for reduction(+ : right_MAE)
  for (int idx : right_index.value()) {
    right_MAE += std::abs(labels[idx] - right_Prediction);
  }
  right_MAE *= (1.0 / right_Population);

  // Compute the result of MAE for the split at position
  double res = ((left_MAE * left_Population) + (right_MAE * right_Population)) *
               (1.0 / base_Population);

  return res;
}

/*
Computes the MAE of two vectors
Parameters : exact results, prediction results
Inputs     : const vector<double>, const vector<double>
Outputs    : double
*/
double MAE::apply(const std::vector<double> &exact,
                  const std::vector<double> &prediction) {

  double res = 0.0;
  int size = prediction.size();
#pragma omp parallel for reduction(+ : res)
  for (int i = 0; i < size; ++i) {
    res += std::abs(exact[i] - prediction[i]);
  }

  // Compute the MAE
  res *= (1.0 / size);

  return res;
}
