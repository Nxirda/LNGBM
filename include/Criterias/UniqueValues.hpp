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
  const std::string name =
      "Unique_Values"; /**< A string variable to display the name. */

  size_t size = 0; /**< A size_t variable that stores the number of elements to
                      generate */

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
  void print() const override;

  /**
   * @brief Returns the number of element the Histogram will compute
   *
   * @return Type : size_t, the number of elements
   */
  size_t get_Criteria_Number() const override;

  /**
   * @brief Static method to get the name of the UniqueValues criteria.
   *
   * @return The name of the UniqueValues criteria.
   */
  std::string get_Name() const override;

  /**
   * @brief Static method to get the name of the UniqueValues criteria.
   *
   * @return The name of the UniqueValues criteria.
   */
  static std::string get_Name_Static();

  /**
   * @brief Computes the unique values of a given data distribution.
   *
   * @param vector<double> list: The distribution of data
   * @param vector<size_t> idx : The index we can access in the data
   * distribution
   * 
   * @return Type: vector<double>, A vector of the unique values
   */
  std::vector<double> compute(const std::vector<double> &list,
                              const std::vector<size_t> &idx) const override;
};

#endif
