#include "EnumCriteria.hpp"

namespace criterias {

//
std::map<std::string, type> dictionary = {{"UD", type::UniformDistribution}, 
                                          {"P", type::Percentiles},
                                          {"Q", type::Quartiles},
                                          {"RV", type::RandomValues},
                                          {"UV", type::UniqueValues}};

//
void print() {

  for (auto const &pair : dictionary) {
    std::cout << "{" << pair.first << "} : ";
    switch (pair.second) {
    case type::UniformDistribution:
      std::cout << UniformDistribution::get_Name_Static();
      break;
    case type::Percentiles:
      std::cout << Percentiles::get_Name_Static();
      break;
    case type::Quartiles:
      std::cout << Quartiles::get_Name_Static();
      break;
    case type::RandomValues:
      std::cout << RandomValues::get_Name_Static();
      break;
    case type::UniqueValues:
      std::cout << UniqueValues::get_Name_Static();
      break;
    default:
      std::cout << " Unknown Type";
    }
    std::cout << "\n";
  }
  return;
}
} // namespace criterias