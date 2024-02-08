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

/**
 * @brief Enumeration representing the different types of splitting criteria.
 *
 * This enumeration defines the implemented types of criteria:
 * Histogram, Percentiles, Quartiles, RandomValues, UniqueValues.
 */
enum class type {
  Histogram,
  Percentiles,
  Quartiles,
  RandomValues,
  UniqueValues,
  Other
};

/**
 * @brief Dictionary mapping string representations to enum values.
 *
 * This map associates string representations with the corresponding enum
 * values from the 'type' enumeration.
 */
inline std::map<std::string, type> dictionary = {{"H", type::Histogram},
                                                 {"P", type::Percentiles},
                                                 {"Q", type::Quartiles},
                                                 {"RV", type::RandomValues},
                                                 {"UV", type::UniqueValues}};

/**
 * @brief Prints the names associated with each enum value.
 *
 * This function iterates through the 'dictionary' map and prints the names
 * associated with each 'type' enum value.
 */
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