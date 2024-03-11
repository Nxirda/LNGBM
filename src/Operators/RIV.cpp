#include <cmath>
#include <omp.h>

#include "RIV.hpp"
#include "TreeNode.hpp"

/********************/
/*                  */
/*    RIV CLASS     */
/*                  */
/********************/

//
RIV::RIV() {}

//
RIV::~RIV() {}

//
void RIV::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

//
std::string RIV::get_Name() const { return this->name; }


//
std::string RIV::get_Name_Static() { return "Reduction In Variance"; }

//
double RIV::compute(size_t position, const DataSet &data,
                    const std::vector<size_t> &index,
                    const double split_Criteria) const {

  // Computes the DataSet Row Indexes that child nodes can access
  auto [left_index, right_index] = data.split(position, split_Criteria, index);

  size_t base_Population = index.size();

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

//
double RIV::apply(const std::vector<double> &exact,
                  const std::vector<double> &prediction) {
  double res = 0.0;
  size_t size = prediction.size();

  for (size_t i = 0; i < size; ++i) {
    res += std::pow((exact[i] - prediction[i]), 2);
  }

  // Compute the Variance
  res *= (1.0 / size);
  return res;
}