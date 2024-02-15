#ifndef UNIQUE_VALUES_H_
#define UNIQUE_VALUES_H_

#include "ICriteria.hpp"
#include <string>

/** @class UniqueValues
 * @brief Class computing the unique values in a given distribution of values.
 *
 * This class implements the ICriteria interface.
 */
class UniqueValues : public ICriteria {
private:
  const std::string name = "Unique_Values";  /**< A string variable to display the name. */

public:
  /**
   * @brief Default constructor for the RandomValues class.
   *
   * Initializes the class with default parameters.
   */
  UniqueValues();

  /**
   * @brief Destructor for the UniqueValues class.
   */
  ~UniqueValues() override;

  /**
   * @brief Prints the name of the class on standard output.
   */
  void print() override;

  /**
   * @brief Static method to get the name of the UniqueValues criteria.
   *
   * @return The name of the UniqueValues criteria.
   */
  static std::string get_Name();

  /**
   * @brief Computes the unique values of a given data distribution.
   *
   * @param vector<double> list: The distribution of data
   * @return Type: vector<double>, A vector of the unique values
   */
  std::vector<double> compute(const std::vector<double> &list) const override;
};

#endif