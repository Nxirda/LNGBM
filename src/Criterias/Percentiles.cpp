#include <algorithm>

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
Print function to see the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void Percentiles::print() {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

/*
Compute the percentiles of the given vector
Parameters : Element distribution
Inputs     : const vector<float>
Outputs    : vector<float>
*/
std::vector<float> Percentiles::compute(const std::vector<float> list) const {

  std::vector<float> percentiles_Values(this->percentiles.size(), 0);
  // Sort the data
  std::vector<float> sorted_Data = list;
  std::sort(sorted_Data.begin(), sorted_Data.end());

  for (size_t i = 0; i < this->percentiles.size(); ++i) {
    float res = sorted_Data[sorted_Data.size() * (this->percentiles[i] / 100)];
    percentiles_Values[i] = res;
  }
  return percentiles_Values;
}
