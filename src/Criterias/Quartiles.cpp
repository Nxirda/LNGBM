#include <algorithm>
#include <execution>
#include <omp.h>
#include <tbb/parallel_sort.h>

#include "Quartiles.hpp"
#include "TrainingElement.hpp"

/************************/
/*                      */
/*    Quartiles CLASS   */
/*                      */
/************************/

/*
Constructor
Parameters :
Inputs     :
Outputs    : Object of Quartiles class
*/
Quartiles::Quartiles() {}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
Quartiles::~Quartiles() {}

/*
Print function to see the name of the criteria
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void Quartiles::print() {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

/*
Return the name of the criteria
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
std::string Quartiles::get_Name() { return "Quartiles"; }

/*
Compute the Quartiles of the given vector
Parameters : Element distribution
Inputs     : const vector<double>
Outputs    : vector<double>
*/
std::vector<double> Quartiles::compute(const std::vector<double> &list) const {

  std::vector<double> quartiles_Values(this->quartiles.size());
  // Sort the data
  int len = list.size();
  std::vector<double> sorted_Data = list;
  std::sort(std::execution::par, sorted_Data.begin(), sorted_Data.end());

#pragma omp parallel for
  for (size_t i = 0; i < this->quartiles.size(); ++i) {
    quartiles_Values[i] =
        sorted_Data[len * (this->quartiles[i] / 100)];
  }
  return quartiles_Values;
}