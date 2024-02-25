#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "ICriteria.hpp"
#include <string>

/**
 * @class Histogram
 * @brief Class computing the Histogram bins of a given value distribution.
 *
 * This class implements the ICriteria interface.
 */
class Histogram : public ICriteria {
private:
  const std::string name =
      "Histogram"; /**< A string variable to display the name. */
  size_t size =
      32; /**< An size_t variable to store the number of bins to generate. (default : 32 bins) */

public:
  /**
   * @brief Default constructor for the Histogram class.
   *
   * Initializes the class with default parameters.
   */
  Histogram();

  /**
   * @brief Parameterized constructor for the Histogram class.
   *
   * @param size_t x: The initial number of bins for the histogram.
   * Initializes the Histogram with a specified number of bins.
   */
  Histogram(size_t x);

  /**
   * @brief Destructor for the Histogram class.
   */
  ~Histogram() override;

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
   * @brief Static method to get the name of the Histogram criteria.
   *
   * @return The name of the Histogram criteria.
   */
  static std::string get_Name();

  /**
   * @brief Computes histogram values for a given data distribution.
   *
   * This method implements the computation of an histogram based on the
   * provided list of data and the bin parameter.
   *
   * @param vector<double> list: The distribution of data
   * @param vector<size_t> idx : The index we can access in the data
   * distribution
   *
   * @return Type: vector<double>, A vector of the values splitting the bins.
   */
  std::vector<double> compute(const std::vector<double> &list,
                              const std::vector<size_t> &idx) const override;
};

#endif