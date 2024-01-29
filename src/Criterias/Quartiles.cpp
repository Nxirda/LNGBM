#include <algorithm>

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
Outputs    :
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
Print function to see the name of the operator
(For debugging mainly)
Parameters :
Inputs     :
Outputs    :
*/
void Quartiles::print() {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

/*
*/
std::vector<float> Quartiles::compute(const std::vector<float> column) const{

    std::vector<float> quartiles_Values(this->quartiles.size(), 0);
    // Sort the data
    std::vector<float> sorted_Data = column;
    std::sort(sorted_Data.begin(), sorted_Data.end());

    for (size_t i = 0; i < this->quartiles.size(); ++i) {
        float res = sorted_Data[sorted_Data.size() * (this->quartiles[i] / 100)];
        quartiles_Values[i] = res;
    }
    return quartiles_Values;
}