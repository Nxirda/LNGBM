#include <cmath>
#include <omp.h>

#include "MAPE.hpp"
#include "TreeNode.hpp"

/********************/
/*                  */
/*    MAPE CLASS    */
/*                  */
/********************/

//
MAPE::MAPE() {}

//
MAPE::~MAPE() {}

//
void MAPE::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}
//
std::string MAPE::get_Name() const { return this->name; }

//
std::string MAPE::get_Name_Static() { return "Mean Absolute Percentage Value"; }

//
double MAPE::compute(size_t position, const DataSet &data,
                     const std::vector<size_t> &index,
                     const double split_Criteria) const {

  // Computes the DataSet Row Indexes that child nodes can access
  auto [left_index, right_index] = data.split(position, split_Criteria, index);

  size_t base_Population = index.size();

  double left_MAPE = 0.0;
  double left_Prediction = data.labels_Mean(left_index.value());
  size_t left_Population = left_index.value().size();

  double right_MAPE = 0.0;
  double right_Prediction = data.labels_Mean(right_index.value());
  size_t right_Population = right_index.value().size();

  const std::vector<double> &labels = data.get_Labels();

  for (size_t idx : left_index.value()) {
    left_MAPE += (std::abs(labels[idx] - left_Prediction)) / left_Prediction;
  }
  left_MAPE *= 100.0;
  left_MAPE *= (1.0 / left_Population);

  for (size_t idx : right_index.value()) {
    right_MAPE += (std::abs(labels[idx] - right_Prediction)) / right_Prediction;
  }
  right_MAPE *= 100.0;
  right_MAPE *= (1.0 / right_Population);

  // Compute the result of MAPE for the split at position
  double res =
      ((left_MAPE * left_Population) + (right_MAPE * right_Population)) /
      base_Population;
  return res;
}

//
double MAPE::apply(const std::vector<double> &exact,
                   const std::vector<double> &prediction) {

  double res = 0.0;
  size_t size = prediction.size();

  for (size_t i = 0; i < size; ++i) {
    res += std::abs(exact[i] - prediction[i]) * (1.0 / exact[i]);
  }

  // Compute the MAPE
  res = (res * 100.0) * (1.0 / size);

  return res;
}
