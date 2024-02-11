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

/*
Constructor
Parameters :
Inputs     :
Outputs    : Object of Unique Values class
*/
UniqueValues::UniqueValues() {}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
UniqueValues::~UniqueValues() {}

/*
Print function to see the name of the criteria
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void UniqueValues::print() {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

/*
Return the name of the criteria
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
std::string UniqueValues::get_Name() { return "Unique Values"; }

/*
Compute the Uniques Values of the given vector
Parameters : Element distribution
Inputs     : const vector<float>
Outputs    : vector<float>
*/
std::vector<float> UniqueValues::compute(const std::vector<float> list) const {

  // Cast in set to get unique values (unordered set of efficiency)
  std::unordered_set<float> unique(list.begin(), list.end());

  // Cast back in array for the result || move to avoid copy
  std::vector<float> res(std::make_move_iterator(unique.begin()),
                         std::make_move_iterator(unique.end()));

  return res;
}