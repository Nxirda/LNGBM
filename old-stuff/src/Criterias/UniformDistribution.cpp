#include <algorithm>
#include <iostream>
#include <limits>

#include "UniformDistribution.hpp"

/*******************************/
/*                             */
/*  UniformDistribution CLASS  */
/*                             */
/*******************************/

//
UniformDistribution::UniformDistribution() {}

//
UniformDistribution::UniformDistribution(size_t x) {
  if (x <= 0) {
    std::cerr << "Can't compute UniformDistribution with x(bins) <= 0\n";
    exit(1);
  } else {
    this->size = x;
  }
}

//
UniformDistribution::~UniformDistribution() {}

//
void UniformDistribution::print() const {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

//
size_t UniformDistribution::get_Criteria_Number() const { return this->size; }

//
std::string UniformDistribution::get_Name() const { return "UD"; }

//
std::string UniformDistribution::get_Name_Static() {
  return "UniformDistribution";
}

//
std::vector<double>
UniformDistribution::compute(const std::vector<double> &list,
                             const std::vector<size_t> &idx) const {

  double min = std::numeric_limits<double>::max();
  double max = 0;
  const size_t size = this->size;

  for (const auto &i : idx) {
    const auto &elem = list[i];
    if (elem < min)
      min = elem;
    else if (elem > max)
      max = elem;
  }

  std::vector<double> bins(size, 0.0);

  const size_t bin_size = (max - min) * (1.0 / size);

  for (size_t i = 0; i < size; ++i) {
    // Returns the values that separates two bins
    bins[i] = min + bin_size * i;
  }

  return bins;
}