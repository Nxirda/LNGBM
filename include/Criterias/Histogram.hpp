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
  // Parameters
  const std::string name =
      "Histogram"; /**< A string variable to display the name. */
  int number_Of_Bins =
      10; /**< An integer variable to store the number of bins to generate. */

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
   * @param int x: The initial number of bins for the histogram.
   * Initializes the Histogram with a specified number of bins.
   */
  Histogram(int x);

  /**
   * @brief Destructor for the Histogram class.
   */
  ~Histogram() override;

  /**
   * @brief Prints the name of the class on standard output.
   */
  void print() override;

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
   * @param vector<float> list: The distribution of data 
   * @return Type: vector<float>, A vector of the values splitting the bins.
   */
  std::vector<float> compute(const std::vector<float> list) const override;
};

#endif