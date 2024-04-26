#ifndef PERCENTILES_H_
#define PERCENTILES_H_

#include "ICriteria.hpp"
#include <string>

/**
 * @class Percentiles
 * @brief Class computing the percentiles of a given value distribution.
 *
 * This class implements the ICriteria interface.
 */
class Percentiles : public ICriteria {
private:
  // Parameters

  const std::vector<double> percentiles = {
      10.0, 20.0, 30.0, 40.0, 50.0,
      60.0, 70.0, 80.0, 90.0}; /**< Array containing the values in % of the
                                  percentiles*/
  const std::string name =
      "Percentiles"; /**< A string variable to display the name. */

  size_t size; /**< A size_t value that stores the number of elements the class
                  computes*/

public:
  /**
   * @brief Default constructor for the Percentiles class.
   *
   * Initializes the class with default parameters.
   */
  Percentiles() noexcept;

  /**
   * @brief Destructor for the Percentiles class.
   */
  ~Percentiles() override;

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
   * @brief Method to get the name of the Percentiles criteria.
   *
   * @return The name of the Percentiles criteria.
   */
  std::string get_Name() const override;

  /**
   * @brief Static method to get the name of the Percentiles criteria.
   *
   * @return The name of the Percentiles criteria.
   */
  static std::string get_Name_Static();

  /**
   * @brief Computes percentiles values for a given data distribution.
   *
   * This method implements the computation of the values of the percentiles
   * of the provided list of data and the bin parameter.
   *
   * @param vector<double> list: The distribution of data
   * @param vector<size_t> idx : The index we can access in the data
   * distribution
   *
   * @return Type: vector<double>, A vector of the values of the percentiles
   */
  std::vector<double> compute(const std::vector<double> &list,
                              const std::vector<size_t> &idx) const override;
};

#endif