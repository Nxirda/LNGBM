#ifndef ENUM_CRITERIA_H_
#define ENUM_CRITERIA_H_

#include <iomanip>
#include <map>

#include "ICriteria.hpp"

#include "Histogram.hpp"
#include "Percentiles.hpp"
#include "Quartiles.hpp"
#include "RandomValues.hpp"
#include "UniqueValues.hpp"

namespace criterias {

/**/
enum class type {
  Histogram,
  Percentiles,
  Quartiles,
  RandomValues,
  UniqueValues,
  Other
};

/**/
inline std::map<std::string, type> dictionary = {{"H", type::Histogram},
                                                 {"P", type::Percentiles},
                                                 {"Q", type::Quartiles},
                                                 {"RV", type::RandomValues},
                                                 {"UV", type::UniqueValues}};

/**/
inline void print() {

  for (auto const &pair : dictionary) {
    std::cout << "{" << pair.first << "} : ";
    switch (pair.second) {
    case type::Histogram:
      std::cout << Histogram::get_Name();
      break;
    case type::Percentiles:
      std::cout << Percentiles::get_Name();
      break;
    case type::Quartiles:
      std::cout << Quartiles::get_Name();
      break;
    case type::RandomValues:
      std::cout << RandomValues::get_Name();
      break;
    case type::UniqueValues:
      std::cout << UniqueValues::get_Name();
      break;
    default:
      std::cout << " Unknown Type";
    }
    std::cout << "\n";
  }
  return;
}
} // namespace criterias

#endif