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
std::string UniqueValues::get_Name() const { return "UV"; }

//
std::string UniqueValues::get_Name_Static() { return "Unique Values"; }

//
std::vector<double>
UniqueValues::compute(const std::vector<double> &list,
                      const std::vector<size_t> &idx) const {

  std::vector<double> list_To_Compute;
  list_To_Compute.reserve(idx.size());

  for (const auto &i : idx) {
    list_To_Compute.push_back(list[i]);
  }

  // Cast in set to get unique values (unordered set for efficiency)
  std::unordered_set<double> unique(list_To_Compute.begin(),
                                    list_To_Compute.end());

  // Cast back in array for the result || move to avoid copy
  std::vector<double> unique_Values(std::make_move_iterator(unique.begin()),
                          std::make_move_iterator(unique.end()));

  return unique_Values;
}