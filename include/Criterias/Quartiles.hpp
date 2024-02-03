#ifndef QUARTILES_H_
#define QUARTILES_H_

#include "ICriteria.hpp"
#include <string>

/** @class Quartiles
 * @brief Class computing the quartiles of a given value distribution.
 *
 * This class implements the ICriteria interface.
 */
class Quartiles : public ICriteria {
private:
  // Parameters
  const std::string name =
      "Quartiles"; /**< A string variable to display the name. */
  const std::vector<float> quartiles = {25.0, 50.0, 75.0}; /**< Array containing
                                  the values in % of the quartiles*/

public:
  /**
   * @brief Default constructor for the Quartiles class.
   *
   * Initializes the class with default parameters.
   */
  Quartiles();

  /**
   * @brief Destructor for the Quartiles class.
   */
  ~Quartiles() override;

  /**
   * @brief Prints the name of the class on standard output.
   */
  void print() override;

  /**
   * @brief Static method to get the name of the Quartiles criteria.
   *
   * @return The name of the Quartiles criteria.
   */
  static std::string get_Name();

  /**
   * @brief Computes quartiles values for a given data distribution.
   *
   * This method implements the computation of the values of the quartiles
   * of the provided list of data and the bin parameter.
   *
   * @param vector<float> list: The distribution of data
   * @return Type: vector<float>, A vector of the values of the quartiles
   */
  std::vector<float> compute(const std::vector<float> list) const override;
};

#endif