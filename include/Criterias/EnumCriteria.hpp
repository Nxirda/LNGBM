#ifndef ENUM_CRITERIA_H_
#define ENUM_CRITERIA_H_

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
  Random_Values,
  Unique_Values,
  Other
};

/**/
inline std::map<std::string, type> dictionnary = {{"H", type::Histogram},
                                                  {"P", type::Percentiles},
                                                  {"Q", type::Quartiles},
                                                  {"RV", type::Random_Values},
                                                  {"UV", type::Unique_Values}};

/**/
inline void print() {
  for (auto const &pair : dictionnary) {
    std::cout << "{" << pair.first << "}\n";
  }
  return;
}
} // namespace criterias

#endif