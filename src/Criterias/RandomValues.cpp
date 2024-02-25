#include <algorithm>

#include "RandomValues.hpp"
#include "TrainingElement.hpp"

/**************************/
/*                        */
/*  RANDOM VALUES CLASS   */
/*                        */
/**************************/

//
RandomValues::RandomValues() {
  std::random_device rd;
  std::mt19937 generator(rd());
  this->gen = generator;
}

//
RandomValues::RandomValues(size_t x) {
  if (x <= 0) {
    // errno = EINVAL;
    // perror("Can't compute split criterias with x <= 0\n x is set to 32");
    std::cerr << "Can't compute split criterias with x <= 0\n x is set to 32\n";
    this->size = 32;
  } else {
    this->size = x;
  }

  std::random_device rd;
  std::mt19937 generator(rd());
  this->gen = generator;
}

//
RandomValues::~RandomValues() {}

//
double RandomValues::get_Random_double(double min, double max) const {
  std::uniform_real_distribution<double> dist(min, max);
  return dist(this->gen);
}

//
void RandomValues::print() const {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

//
size_t RandomValues::get_Criteria_Number() const { return this->size; }

//  
std::string RandomValues::get_Name() { return "Random Values"; }

//
std::vector<double>
RandomValues::compute(const std::vector<double> &list,
                      const std::vector<size_t> &idx) const {

  // auto min = std::min_element(std::execution::par, list.begin(), list.end());
  // auto max = std::max_element(std::execution::par, list.begin(), list.end());
  auto min = std::min_element(list.begin(), list.end());
  auto max = std::max_element(list.begin(), list.end());

  if (this->size < 0)
    return {};

  std::vector<double> res(this->size, 0);
  res[0] = *min;
  res[this->size - 1] = *max;

  for (size_t i = 1; i < this->size - 1; ++i) {
    res[i] = get_Random_double(*min, *max);
  }
  return res;
}