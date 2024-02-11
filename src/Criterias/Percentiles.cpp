#include <algorithm>
#include <execution>
#include <omp.h>
#include <tbb/parallel_sort.h>

#include "Percentiles.hpp"
#include "TrainingElement.hpp"

/************************/
/*                      */
/*  Percentiles CLASS   */
/*                      */
/************************/

/*
Constructor
Parameters :
Inputs     :
Outputs    : Object of Percentile class
*/
Percentiles::Percentiles() {}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
Percentiles::~Percentiles() {}

/*
Print function to see the name of the criteria
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void Percentiles::print() {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

/*
Return the name of the criteria
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
std::string Percentiles::get_Name() { return "Percentiles"; }

/*
Compute the percentiles of the given vector
Parameters : Element distribution
Inputs     : const vector<float>
Outputs    : vector<float>
*/
std::vector<float> Percentiles::compute(const std::vector<float> list) const {

  std::vector<float> percentiles_Values(this->percentiles.size(), 0);
  // Sort the data
  int len = list.size();
  std::vector<float> sorted_Data = list;
  std::sort(std::execution::par, sorted_Data.begin(), sorted_Data.end());

#pragma omp parallel for
  for (size_t i = 0; i < this->percentiles.size(); ++i) {
    percentiles_Values[i] = sorted_Data[len * (this->percentiles[i] / 100)];
  }
  return percentiles_Values;
}
