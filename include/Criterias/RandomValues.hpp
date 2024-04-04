#ifndef RANDOM_VALUES_H_
#define RANDOM_VALUES_H_

#include "ICriteria.hpp"
#include <random>
#include <string>

/** @class RandomValues
 * @brief Class computing some random values in a given distribution of values.
 *
 * This class implements the ICriteria interface.
 */
class RandomValues : public ICriteria {
private:
  // Parameters
  mutable std::mt19937 gen; /**< A mutable random value generator variable to
                               store the randomness */
  const std::string name =
      "Random_Values"; /**< A string variable to display the name. */
  size_t size = 32; /**< Integer representing the number of values to compute.
                       (default : 32)*/

public:
  /**
   * @brief Default constructor for the RandomValues class.
   *
   * Initializes the class with default parameters.
   */
  RandomValues();

  /**
   * @brief Parameterized constructor for the RandomValues class.
   *
   * @param size_t x: The number of values to compute.
   */
  RandomValues(size_t x);

  /**
   * @brief Destructor for the RandomValues class.
   */
  ~RandomValues() override;

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
   * @brief Method to get the name of the RandomValues criteria.
   *
   * @return The name of the RandomValues criteria.
   */
  std::string get_Name() const override;

  /**
   * @brief Static method to get the name of the RandomValues criteria.
   *
   * @return The name of the RandomValues criteria.
   */
  static std::string get_Name_Static();

  /**
   * @brief Computes random values in given data distribution.
   *
   * This method implements the computation of number_Of_Elements of random
   * values in the provided list of data.
   *
   * @param vector<double> list: The distribution of data
   * @param vector<size_t> idx : The index we can access in the data
   * distribution
   *
   * @return Type: vector<double>, A vector of the random values
   */
  std::vector<double> compute(const std::vector<double> &list,
                              const std::vector<size_t> &idx) const override;

  /**
   * @brief Computes a random double in given boundaries
   *
   * @param double min: minimal value we can return
   * @param double max: maximal value we can return
   *
   * @return Type: double, A random flaot
   */
  double get_Random_double(double min, double max) const;
};

#endif