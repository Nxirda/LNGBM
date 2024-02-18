#include <algorithm>
#include <execution>
#include <omp.h>

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
Outputs    : Object of Histogram class
*/
Histogram::Histogram() {}

/*
Constructor with argument for the number of bins
Parameters : number of bins
Inputs     : int
Outputs    : Object of Histogram class
*/
Histogram::Histogram(int x) {
  if (x <= 0) {
    /* errno = EINVAL;
    perror("Can't compute split criterias with x <= 0\n x is set to 5"); */
    std::cerr << "Can't compute split criterias with x <= 0\n x is set to 10\n";
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
Print function to see the name of the criteria
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void Histogram::print() {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

/*
Return the name of the criteria
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
std::string Histogram::get_Name() { return "Histogram"; }

/*
Compute the historgam of the given vector
Parameters : Element distribution
Inputs     : const vector<double>
Outputs    : vector<double>
*/
std::vector<double> Histogram::compute(const std::vector<double> &list) const {

  /* auto min = std::min_element(std::execution::par_unseq, list.begin(), list.end());
  auto max = std::max_element(std::execution::par_unseq, list.begin(), list.end()); */

  auto min = std::min_element(list.begin(), list.end());
  auto max = std::max_element(list.begin(), list.end());

  std::vector<double> res(this->number_Of_Bins, 0.0);

  int bin_size = (*max - *min) * (1.0 / this->number_Of_Bins);

#pragma omp parallel for
  for (int i = 0; i < this->number_Of_Bins; ++i) {
    // Returns the values that separates two bins
    res[i] = *min + bin_size * i;
  }
  return res;
}