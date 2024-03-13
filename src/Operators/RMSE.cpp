#include <cblas.h>
#include <cmath>
#include <omp.h>

#include "RMSE.hpp"

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
std::string RMSE::get_Name() const { return this->name; }

//
std::string RMSE::get_Name_Static() { return "Root Mean Square Error"; }

//
double RMSE::compute(const std::vector<double> &exact,
                     double prediction) const {
  double res = 0.0;
  size_t size = exact.size();

  std::vector<double> prediction_Vector(size, (prediction));

  std::vector<double> absoluteDifferences(size);
  cblas_dcopy(size, exact.data(), 1, absoluteDifferences.data(), 1);

  // Computes the yi - ŷi part of the RMSE
  cblas_daxpy(size, -1.0, prediction_Vector.data(), 1,
              absoluteDifferences.data(), 1);

  // RMSE computing
  res = cblas_dnrm2(static_cast<int>(size), absoluteDifferences.data(), 1);

  return res;
}