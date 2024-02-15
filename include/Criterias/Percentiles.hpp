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
  const std::string name =
      "Percentiles"; /**< A string variable to display the name. */
  const std::vector<double> percentiles = {
      10.0, 20.0, 30.0, 40.0, 50.0,
      60.0, 70.0, 80.0, 90.0}; /**< Array containing the values in % of the
                                  percentiles*/

public:
  /**
   * @brief Default constructor for the Percentiles class.
   *
   * Initializes the class with default parameters.
   */
  Percentiles();

  /**
   * @brief Destructor for the Percentiles class.
   */
  ~Percentiles() override;

  /**
   * @brief Prints the name of the class on standard output.
   */
  void print() override;

  /**
   * @brief Static method to get the name of the Percentiles criteria.
   *
   * @return The name of the Percentiles criteria.
   */
  static std::string get_Name();

  /**
   * @brief Computes percentiles values for a given data distribution.
   *
   * This method implements the computation of the values of the percentiles
   * of the provided list of data and the bin parameter.
   *
   * @param vector<double> list: The distribution of data 
   * @return Type: vector<double>, A vector of the values of the percentiles
   */
  std::vector<double> compute(const std::vector<double> &list) const override;
};

#endif