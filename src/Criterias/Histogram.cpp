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
    /* errno = EINVAL;
    perror("Can't compute split criterias with x <= 0\n x is set to 5"); */
    std::cerr << "Can't compute Histogram with x(bins) <= 0\n x is set to default (32)\n";
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
std::string Histogram::get_Name() { return "Histogram"; }

//
std::vector<double> Histogram::compute(const std::vector<double> &list,
                                       const std::vector<size_t> &idx) const {

  std::vector<double> curr_Vector;
  curr_Vector.reserve(idx.size());

  // Copy of the current vector might just compute the min here too
  for (size_t i : idx)
    curr_Vector.push_back(list[i]);

  auto min = std::min_element(curr_Vector.begin(), curr_Vector.end());
  auto max = std::max_element(curr_Vector.begin(), curr_Vector.end());

  std::vector<double> res(this->size, 0.0);

  size_t bin_size = (*max - *min) * (1.0 / this->size);

  for (size_t i = 0; i < this->size; ++i) {
    // Returns the values that separates two bins
    res[i] = *min + bin_size * i;
  }
  return res;
}