#ifndef UNIFORM_DISTRIBUTION_H_
#define UNIFORM_DISTRIBUTION_H_

#include "ICriteria.hpp"
#include <string>

/**
 * @class UniformDistribution
 * @brief Class computing the UniformDistribution bins of a given value distribution.
 *
 * This class implements the ICriteria interface.
 */
class UniformDistribution : public ICriteria {
private:
  const std::string name =
      "UniformDistribution"; /**< A string variable to display the name. */
  size_t size =
      256; /**< An size_t variable to store the number of values to generate. (default : 256 values) */

public:
  /**
   * @brief Default constructor for the UniformDistribution class.
   *
   * Initializes the class with default parameters.
   */
  UniformDistribution();

  /**
   * @brief Parameterized constructor for the UniformDistribution class.
   *
   * @param size_t x: The initial number of values for the UniformDistribution.
   * Initializes the UniformDistribution with a specified number of values.
   */
  UniformDistribution(size_t x);

  /**
   * @brief Destructor for the UniformDistribution class.
   */
  ~UniformDistribution() override;

  /**
   * @brief Prints the name of the class on standard output.
   */
  void print() const override;

  /**
   * @brief Returns the number of element the UniformDistribution will compute
   *
   * @return Type : size_t, the number of elements
   */
  size_t get_Criteria_Number() const override;


  /**
   * @brief Method to get the name of the UniformDistribution criteria.
   *
   * @return The name of the UniformDistribution criteria.
   */
  std::string get_Name() const override;

  /**
   * @brief Static method to get the name of the UniformDistribution criteria.
   *
   * @return The name of the UniformDistribution criteria.
   */
  static std::string get_Name_Static();

  /**
   * @brief Computes UniformDistribution values for a given data distribution.
   *
   * This method implements the computation of an UniformDistribution based on the
   * provided list of data and the size parameter.
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