#ifndef MAPE_H_
#define MAPE_H_

#include "IOperator.hpp"
#include <string>

/**
 * @class MAPE
 * @brief A class used to compute the Mean Absolute Percentage Error on a
 * dataset
 *
 * This class implements the IOperator interface
 */
class MAPE : public IOperator {

private:
  const std::string name = "MAPE"; /**< A string variable representing the name
                              of the class */

public:
  /**
   * @brief Default constructor for the MAPE class.
   *
   * Initializes the class with default parameters.
   */
  MAPE();

  /**
   * @brief Destructor for the MAPE class
   */
  ~MAPE() override;

  /**
   * @brief Prints the name of the class on standard output.
   */
  void print() override;

  /**
   * @brief Static methods that returns the name of the class (MAPE)
   */
  static std::string get_Name();

  /**
   * @brief Computes the result of the MAPE on the column at position folowwing
   * a split defined by the split criteria
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
  double compute(size_t position, const DataSet &data,
                 const std::vector<size_t> &index,
                 const double split_Criteria) const override;

  /**
   * @brief Computes the MAPE for a given data distribution.
   *
   * @param vector<double> prediction : The prediction we made
   * @param vector<size_t> idx : The real result
   *
   * @return Type: double, The MAE
   */
  static double apply(const std::vector<double> &exact,
                      const std::vector<double> &prediction);
};

#endif