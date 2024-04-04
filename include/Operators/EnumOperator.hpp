#ifndef ENUM_OPERATOR_H_
#define ENUM_OPERATOR_H_

#include <map>

#include "IOperator.hpp"

#include "MAE.hpp"
#include "MAPE.hpp"
#include "RIV.hpp"
#include "RMSE.hpp"

namespace operators {

/**
 * @brief Enumeration representing the different types of Operators used for splitting
 *
 * This enumeration should contain the implemented types of criteria:
 * 
 * Mean Absolute Error, Mean Absolute Percentage Error
 * Root Mean Square Error, Reduction In Variance
 */
enum class type { MAE, MAPE, RMSE, RIV, Other };

/**
 * @brief Dictionary mapping string representations to enum values.
 *
 * This map associates string representations with the corresponding enum
 * values from the 'type' enumeration.
 */
extern std::map<std::string, type> dictionnary;

/**
 * @brief Prints the names associated with each enum value.
 *
 * This function iterates through the 'dictionary' map and prints the names
 * associated with each 'type' enum value.
 */
void print();
} // namespace operators

#endif