#include <algorithm>
#include <random>
#include <unordered_set>

#include "TrainingElement.hpp"
#include "UniqueValues.hpp"

/**************************/
/*                        */
/*  RANDOM VALUES CLASS   */
/*                        */
/**************************/

//
UniqueValues::UniqueValues() {}

//
UniqueValues::~UniqueValues() {}

//
void UniqueValues::print() const {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

//
size_t UniqueValues::get_Criteria_Number() const { return this->size; };

//
std::string UniqueValues::get_Name() { return "Unique Values"; }

//
std::vector<double>
UniqueValues::compute(const std::vector<double> &list,
                      const std::vector<size_t> &idx) const {

  // Cast in set to get unique values (unordered set of efficiency)
  std::unordered_set<double> unique(list.begin(), list.end());

  // Cast back in array for the result || move to avoid copy
  std::vector<double> res(std::make_move_iterator(unique.begin()),
                          std::make_move_iterator(unique.end()));

  return res;
}