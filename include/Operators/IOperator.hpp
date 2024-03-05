#ifndef I_OPERATOR_H_
#define I_OPERATOR_H_

#include <memory>
#include <string>

#include "DataSet.hpp"

/**
 * @interface IOperator
 * @brief A simple interface to build Operators
 *
 * This interface defines a set of methods that operators should
 * adhere to.
 */
class IOperator {

private:
  const std::string name; /**< A string variable representing the name of the
                             class : shall be initialized by default. */

public:
  /**
   * @brief Destructor for the class
   */
  virtual ~IOperator(){};

  /**
   * @brief Should print the name of the class on standard output.
   */
  virtual void print() = 0;

  /**
   * @brief Computes a value defined by the operator implementing it.
   *
   * @param size_t position: the feature of the dataset on which to compute
   * @param const DataSet & data : The dataset
   * @param const std::vector<size_t> &index : a vector of the index of the
   * element we can access
   * @param const double criteria : the splitting criteria on which to split the
   * datas
   *
   * @return Type: double, A value representing the result of the metric on the
   * parameters.
   */
  virtual double compute(size_t position, const DataSet &data,
                         const std::vector<size_t> &index,
                         const double criteria) const = 0;
};

#endif