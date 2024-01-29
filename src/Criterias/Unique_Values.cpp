#include <algorithm>
#include <random>
#include <unordered_set>

#include "TrainingElement.hpp"
#include "Unique_Values.hpp"

/**************************/
/*                        */
/*  RANDOM VALUES CLASS   */
/*                        */
/**************************/

/*
Constructor
Parameters :
Inputs     :
Outputs    :
*/
Unique_Values::Unique_Values() {}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
Unique_Values::~Unique_Values() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void Unique_Values::print() {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

/*
 */
std::vector<float>
Unique_Values::compute(const std::vector<float> column) const {

  // Cast in set to get unique values (unordered set of efficiency)
  std::unordered_set<float> unique(column.begin(), column.end());

  // Cast back in array for the result
  std::vector<float> res(unique.begin(), unique.end());

  return res;
}