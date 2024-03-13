#include <cblas.h>
#include <cmath>
#include <omp.h>

#include "MAE.hpp"

/********************/
/*                  */
/*    MAE CLASS     */
/*                  */
/********************/

//
MAE::MAE() {}

//
MAE::~MAE() {}

//
void MAE::print() {
  std::cout << "=== Operator is : " << this->name << " ===\n";
}

//
std::string MAE::get_Name() const { return this->name; }

//
std::string MAE::get_Name_Static() { return "Mean Absolute Value"; }

//
double MAE::compute(const std::vector<double> &exact, double prediction) const {

  double res = 0.0;
  size_t size = exact.size();

  std::vector<double> prediction_Vector(size, (prediction));

  std::vector<double> absoluteDifferences(size);
  // Copy necessary for daxpy
  cblas_dcopy(size, exact.data(), 1, absoluteDifferences.data(), 1);

  // Computes the yi - ŷi part of the MAE
  cblas_daxpy(size, -1.0, prediction_Vector.data(), 1,
              absoluteDifferences.data(), 1);

  // MAE computing (dasum takes the absolute values)
  res = cblas_dasum(static_cast<int>(size), absoluteDifferences.data(), 1);
  res *= 1.0 / size;
  return res;
}

//
double MAE::apply(const std::vector<double> &exact,
                  const std::vector<double> &prediction) {

  double res = 0.0;
  size_t size = exact.size();

  std::vector<double> result_Vector(size);
  cblas_dcopy(size, exact.data(), 1, result_Vector.data(), 1);

  // Computes the yi - ŷi part of the MAE
  cblas_daxpy(size, -1.0, prediction.data(), 1, result_Vector.data(), 1);

  // MAE computing
  res = cblas_dasum(static_cast<int>(size), result_Vector.data(), 1);

  res *= (1.0 / size);
  return res;
}