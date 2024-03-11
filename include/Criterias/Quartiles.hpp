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
  const std::vector<double> quartiles = {25.0, 50.0, 75.0}; /**< Array
                                  containing the values in % of the quartiles*/

  size_t size;

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
  void print() const override;

  /**
   * @brief Returns the number of element the Histogram will compute
   *
   * @return Type : size_t, the number of elements
   */
  size_t get_Criteria_Number() const override;

  /**
   * @brief Method to get the name of the Quartiles criteria.
   *
   * @return The name of the Quartiles criteria.
   */
  std::string get_Name() const override;
  
  /**
   * @brief Static method to get the name of the Quartiles criteria.
   *
   * @return The name of the Quartiles criteria.
   */
  static std::string get_Name_Static();

  /**
   * @brief Computes quartiles values for a given data distribution.
   *
   * This method implements the computation of the values of the quartiles
   * of the provided list of data and the bin parameter.
   *
   * @param vector<double> list: The distribution of data
   * @param vector<size_t> idx : The index we can access in the data
   * distribution
   *
   * @return Type: vector<double>, A vector of the values of the quartiles
   */
  std::vector<double> compute(const std::vector<double> &list,
                              const std::vector<size_t> &idx) const override;
};

#endif