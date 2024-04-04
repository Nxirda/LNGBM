#include <cblas.h>
#include <cmath>
#include <omp.h>

#include "RIV.hpp"

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
double RIV::compute(const std::vector<double> &exact, double prediction) const {
  double res = 0.0;
  const size_t size = exact.size();

  std::vector<double> prediction_Vector(size, prediction);

  std::vector<double> absoluteDifferences(size);
  // Copy necessary for daxpy
  cblas_dcopy(size, exact.data(), 1, absoluteDifferences.data(), 1);

  // Computes the yi - ŷi part of the MAPE
  cblas_daxpy(size, -1.0, prediction_Vector.data(), 1,
              absoluteDifferences.data(), 1);

  // Square the result
  res = cblas_ddot(size, absoluteDifferences.data(), 1,
                   absoluteDifferences.data(), 1);

  // Compute the Variance
  res *= (1.0 / size);
  return res;
}

//
double RIV::apply(const std::vector<double> &exact,
                  const std::vector<double> &prediction) {
  double res = 0.0;
  const size_t size = prediction.size();

  for (size_t i = 0; i < size; ++i) {
    res += std::pow((exact[i] - prediction[i]), 2);
  }

  // Compute the Variance
  res *= (1.0 / size);
  return res;
}