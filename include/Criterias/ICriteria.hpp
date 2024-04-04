#ifndef I_CRITERIA_H_
#define I_CRITERIA_H_

#include <string>
#include <vector>

/**
 * @interface ICriteria
 * @brief A simple interface to build Criterias
 *
 * This interface defines a set of methods that criterias should
 * adhere to.
 */
class ICriteria {

private:
  const std::string name; /**< A string variable representing the name of the
                             class : shall be initialized by default. */

  size_t size; /**< A float variable containing the number of values to get : it
                  shall be initialized by default*/

public:
  /**
   * @brief Destructor for the class
   */
  virtual ~ICriteria(){};

  /**
   * @brief Should print the name of the class on standard output.
   */
  virtual void print() const = 0;

  /**
   * @brief Should return the name of the Criteria
   */
  virtual std::string get_Name() const = 0;

  /**
   * @brief Returns the number of element the criteria will compute
   *
   * @return Type : size_t, the number of elements
   */
  virtual size_t get_Criteria_Number() const = 0;

  /**
   * @brief Computes a vector of values defined by the criteria implementing it.
   *
   * @param vector<double> list: The distribution of data
   * @param vector<size_t> idx : The index we can access in the data
   * distribution
   *
   * @return Type: vector<double>, A vector of values contained in the
   * distribution.
   */
  virtual std::vector<double> compute(const std::vector<double> &list,
                                      const std::vector<size_t> &idx) const = 0;
};

#endif