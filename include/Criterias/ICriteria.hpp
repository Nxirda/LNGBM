#ifndef I_CRITERIA_H_
#define I_CRITERIA_H_

#include <memory>
#include <string>

#include "DataSet.hpp"

/**
 * @interface ICriteria
 * @brief A simple interface to build Criterias
 *
 * This interface defines a set of methods that classes implementing it should
 * adhere to.
 */
class ICriteria {

private:
  const std::string name; /**< A string variable representing the name of the
                             class : shall be initialized by default. */

public:
  /**
   * @brief Destructor for the class
   */
  virtual ~ICriteria(){};

  /**
   * @brief Computes a vector of values defined by the criteria implementing it.
   *
   * @param vector<float> list: The distribution of data
   * @return Type: vector<float>, A vector of values contained in the
   * distribution.
   */
  virtual std::vector<float> compute(const std::vector<float> list) const = 0;

  /**
   * @brief Should print the name of the class on standard output.
   */
  virtual void print() = 0;
};

#endif