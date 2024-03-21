#include <algorithm>

#include "Histogram.hpp"
#include "TrainingElement.hpp"

/**************************/
/*                        */
/*    HISTOGRAM CLASS     */
/*                        */
/**************************/

//
Histogram::Histogram() {}

//
Histogram::Histogram(size_t x) {
  if (x <= 0) {
    std::cerr << "Can't compute Histogram with x(bins) <= 0\n x is set to "
                 "default (32)\n";
  } else {
    this->size = x;
  }
}

//
Histogram::~Histogram() {}

//
void Histogram::print() const {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

//
size_t Histogram::get_Criteria_Number() const { return this->size; }

//
std::string Histogram::get_Name() const { return "H"; }

//
std::string Histogram::get_Name_Static() { return "Histogram"; }

//
std::vector<double> Histogram::compute(const std::vector<double> &list,
                                       const std::vector<size_t> &idx) const {

  double min = std::numeric_limits<double>::max();
  double max = 0;

  for (const auto &i : idx) {
    const auto &elem = list[i];
    if (elem < min)
      min = elem;
    else if (elem > max)
      max = elem;
  }

  std::vector<double> bins(this->size, 0.0);

  size_t bin_size = (max - min) * (1.0 / this->size);

  for (size_t i = 0; i < this->size; ++i) {
    // Returns the values that separates two bins
    bins[i] = min + bin_size * i;
  }

  return bins;
}