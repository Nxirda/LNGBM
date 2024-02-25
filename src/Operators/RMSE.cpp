#include <cmath>
#include <omp.h>

#include "RMSE.hpp"
#include "TreeNode.hpp"

/********************/
/*                  */
/*    RMSE CLASS    */
/*                  */
/********************/

//
RMSE::RMSE() {}

//
RMSE::~RMSE() {}

//
void RMSE::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

//
std::string RMSE::get_Name() { return "Root Mean Square Error"; }

//
double RMSE::compute(size_t position, const DataSet &data,
                     const std::vector<size_t> &index,
                     const double split_Criteria) const {

  // Computes the DataSet Row Indexes that child nodes can access
  auto [left_index, right_index] = data.split(position, split_Criteria, index);

  size_t base_Population = index.size();

  double left_RMSE = 0.0;
  TreeNode left_Child{};
  double left_Prediction = data.labels_Mean(left_index.value());
  size_t left_Population = left_index.value().size();

  double right_RMSE = 0.0;
  TreeNode right_Child{};
  double right_Prediction = data.labels_Mean(right_index.value());
  size_t right_Population = right_index.value().size();

  for (size_t idx : left_index.value()) {
    left_RMSE += pow((std::abs(data.get_Labels()[idx] - left_Prediction)), 2) *
                 (1.0 / left_Population);
  }
  left_RMSE = sqrt(left_RMSE);

  for (size_t idx : right_index.value()) {
    right_RMSE +=
        pow((std::abs(data.get_Labels()[idx] - right_Prediction)), 2) *
        (1.0 / right_Population);
  }
  right_RMSE = sqrt(right_RMSE);

  // Compute the result of RMSE for the split at position
  double res =
      ((left_RMSE * left_Population) + (right_RMSE * right_Population)); // *

  res *= (1.0 / base_Population);
  return res;
}