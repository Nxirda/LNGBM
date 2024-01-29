#include <algorithm>

#include "Histogram.hpp"
#include "TrainingElement.hpp"

/**************************/
/*                        */
/*    HISTOGRAM CLASS     */
/*                        */
/**************************/

/*
Constructor
Parameters :
Inputs     :
Outputs    :
*/
Histogram::Histogram() {}

/*
 */
Histogram::Histogram(int x) {
  if (x <= 0) {
    errno = EINVAL;
    perror("Can't compute split criterias with x <= 0\n x is set to 5");
    this->number_Of_Bins = 10;
  } else {
    this->number_Of_Bins = x;
  }
}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
Histogram::~Histogram() {}

/*
Print function to see the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void Histogram::print() {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

/*
 */
std::vector<float> Histogram::compute(const std::vector<float> column) const {

  auto min = std::min_element(column.begin(), column.end());
  auto max = std::max_element(column.begin(), column.end());

  std::vector<float> res(this->number_Of_Bins, 0.0);

  int bin_size = (*max - *min) / this->number_Of_Bins;

  for (int i = 0; i < this->number_Of_Bins; ++i) {
    // Returns the values that separates two bins
    res[i] = *min + bin_size * i;
  }
  return res;
}