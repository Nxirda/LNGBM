#ifndef ENUM_CRITERIA_H_
#define ENUM_CRITERIA_H_

#include <iomanip>
#include <iostream>
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
 * This enumeration should contain the implemented types of criteria:
 *
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
extern std::map<std::string, type> dictionary;

/**
 * @brief Prints the names associated with each enum value.
 *
 * This function iterates through the 'dictionary' map and prints the names
 * associated with each 'type' enum value.
 */
void print();

} // namespace criterias

#endif