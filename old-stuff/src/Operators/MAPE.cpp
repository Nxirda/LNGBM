#include <cblas.h>
#include <cmath>
#include <omp.h>

#include "MAPE.hpp"

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

double MAPE::compute(const std::vector<double> &exact,
                     double prediction) const {

  double res = 0.0;
  const size_t size = exact.size();

  std::vector<double> prediction_Vector(size, (prediction));

  std::vector<double> absoluteDifferences(size);
  // Copy necessary for daxpy
  cblas_dcopy(size, exact.data(), 1, absoluteDifferences.data(), 1);

  // Computes the yi - ŷi part of the MAPE
  cblas_daxpy(size, -1.0, prediction_Vector.data(), 1,
              absoluteDifferences.data(), 1);

  // Computes the yi / N part of the MAPE
  cblas_dscal(size, 1.0 / prediction, absoluteDifferences.data(), 1);

  // Computes the final reduction of the MAPE (dasum takes the absolute values)
  res = cblas_dasum(static_cast<int>(size), absoluteDifferences.data(), 1);

  res = (res * 100.0) * (1.0 / size);
  return res;
}

//
double MAPE::apply(const std::vector<double> &exact,
                   const std::vector<double> &prediction) {

  // Figure out how to do the division part with blas when we have two vectors
  double res = 0.0;
  const size_t size = prediction.size();

  for (size_t i = 0; i < size; ++i) {
    res += std::abs(exact[i] - prediction[i]) * (1.0 / exact[i]);
  }

  // Compute the MAPE
  res = (res * 100.0) * (1.0 / size);

  return res;
}

/* double res = 0.0;
  size_t size = exact.size();

  //std::vector<double> prediction_Vector(size, (prediction));

  std::vector<double> absoluteDifferences(size);
  // Copy necessary for daxpy
  cblas_dcopy(size, exact.data(), 1, absoluteDifferences.data(), 1);

  // Computes the yi - ŷi part of the MAPE
  cblas_daxpy(size, -1.0, prediction.data(), 1,
              absoluteDifferences.data(), 1);

  cblas_dscal(size, 1.0 / prediction, absoluteDifferences.data(), 1);

  // MAE computing (dasum takes the absolute values)
  res = cblas_dasum(static_cast<int>(size), absoluteDifferences.data(), 1);

  res = (res * 100.0) * (1.0 / size);
  return res; */