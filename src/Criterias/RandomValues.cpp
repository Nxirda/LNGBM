#include <algorithm>

#include "RandomValues.hpp"
#include "TrainingElement.hpp"

/**************************/
/*                        */
/*  RANDOM VALUES CLASS   */
/*                        */
/**************************/

//
RandomValues::RandomValues() noexcept {
  std::random_device rd;
  std::mt19937 generator(rd());
  this->gen = generator;
}

//
RandomValues::RandomValues(size_t x) {
  if (x <= 0) {
    std::cerr << "Can't compute split criterias with x <= 0\n";
    this->size = 256;
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
std::string RandomValues::get_Name() const { return "RV"; }

//
std::string RandomValues::get_Name_Static() { return "Random Values"; }

//
std::vector<double>
RandomValues::compute(const std::vector<double> &list,
                      const std::vector<size_t> &idx) const {

  std::vector<double> list_To_Compute;
  list_To_Compute.reserve(idx.size());

  for (const auto &i : idx) {
    list_To_Compute.push_back(list[i]);
  }

  const auto min = std::min_element(list_To_Compute.begin(), list_To_Compute.end());
  const auto max = std::max_element(list_To_Compute.begin(), list_To_Compute.end());

  std::vector<double> random_Values(this->size, 0);
  random_Values[0] = *min;
  random_Values[this->size - 1] = *max;

  for (size_t i = 1; i < this->size - 1; ++i) {
    random_Values[i] = get_Random_double(*min, *max);
  }
  return random_Values;
}