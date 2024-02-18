#include <algorithm>
#include <execution>
#include <omp.h>
#include <random>

#include "RandomValues.hpp"
#include "TrainingElement.hpp"

/**************************/
/*                        */
/*  RANDOM VALUES CLASS   */
/*                        */
/**************************/

/*
Constructor
Parameters :
Inputs     :
Outputs    : Object of Random_Values class
*/
RandomValues::RandomValues() {}

/*
Constructor with argument for the number of values to compute
Parameters : number of values
Inputs     : int
Outputs    : Object of Random Values class
*/
RandomValues::RandomValues(int x) {
  if (x <= 0) {
    errno = EINVAL;
    perror("Can't compute split criterias with x <= 0\n x is set to 5");
    this->number_Of_Elements = 10;
  } else {
    this->number_Of_Elements = x;
  }
}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
RandomValues::~RandomValues() {}

/*
Computes a random double between min and max
Parameters : minimum value, maximum value
Inputs     : double, double
Outputs    : double
 */
double RandomValues::get_Random_double(double min, double max) const {
  // Hardware based entropy
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<double> dist(min, max);
  return dist(gen);
}

/*
Print function to see the name of the criteria
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void RandomValues::print() {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

/*
Return the name of the criteria
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
std::string RandomValues::get_Name() { return "Random Values"; }

/*
Compute random values in the given vector
Parameters : Element distribution
Inputs     : const vector<double>
Outputs    : vector<double>
*/
std::vector<double>
RandomValues::compute(const std::vector<double> &list) const {

  auto min = std::min_element(std::execution::par, list.begin(), list.end());
  auto max = std::max_element(std::execution::par, list.begin(), list.end());

  if (this->number_Of_Elements < 0)
    return {};

  std::vector<double> res(this->number_Of_Elements, 0);
  res[0] = *min;
  res[this->number_Of_Elements - 1] = *max;

#pragma omp parallel for
  for (int i = 1; i < this->number_Of_Elements - 1; ++i) {
    res[i] = get_Random_double(*min, *max);
  }
  return res;
}