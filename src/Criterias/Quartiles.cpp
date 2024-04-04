#include <algorithm>

#include "Quartiles.hpp"
#include "TrainingElement.hpp"

/************************/
/*                      */
/*    Quartiles CLASS   */
/*                      */
/************************/

//
Quartiles::Quartiles() { this->size = this->quartiles.size(); }

//
Quartiles::~Quartiles() {}

//
void Quartiles::print() const {
  std::cout << "=== Criteria is : " << this->name << " ===\n";
}

//
size_t Quartiles::get_Criteria_Number() const { return this->size; };

//
std::string Quartiles::get_Name() const { return "Q"; }

//
std::string Quartiles::get_Name_Static() { return "Quartiles"; }

//
std::vector<double> Quartiles::compute(const std::vector<double> &list,
                                       const std::vector<size_t> &idx) const {

  std::vector<double> quartiles_Values(this->quartiles.size());

  std::vector<double> sorted_Data;
  sorted_Data.reserve(idx.size());

  for (const auto &i : idx) {
    sorted_Data.push_back(list[i]);
  }

  // Sort the data
  const size_t length = sorted_Data.size();
  std::sort(sorted_Data.begin(), sorted_Data.end());

  for (size_t i = 0; i < this->quartiles.size(); ++i) {
    quartiles_Values[i] = sorted_Data[length * (this->quartiles[i] / 100)];
  }
  return quartiles_Values;
}