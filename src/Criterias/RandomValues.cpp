#include <algorithm>
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
Random_Values::Random_Values() {}

/*
Constructor with argument for the number of values to compute
Parameters : number of values
Inputs     : int
Outputs    : Object of Random Values class
*/
Random_Values::Random_Values(int x) {
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
Random_Values::~Random_Values() {}

/*
Computes a random float between min and max
Parameters : minimum value, maximum value
Inputs     : float, float
Outputs    : float
 */
float Random_Values::get_Random_Float(float min, float max) const {
  // Hardware based entropy
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<float> dist(min, max);
  return dist(gen);
}

/*
Print function to see the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void Random_Values::print() {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

/*
Compute random values in the given vector
Parameters : Element distribution
Inputs     : const vector<float>
Outputs    : vector<float>
*/
std::vector<float> Random_Values::compute(const std::vector<float> list) const {

  auto min = std::min_element(list.begin(), list.end());
  auto max = std::max_element(list.begin(), list.end());

  std::vector<float> res(this->number_Of_Elements, 0);
  res[0] = *min;
  res[this->number_Of_Elements - 1] = *max;

  if (this->number_Of_Elements > 0) {
    for (int i = 1; i < this->number_Of_Elements - 1; ++i) {
      res[i] = get_Random_Float(*min, *max);
    }
  }
  return res;
}