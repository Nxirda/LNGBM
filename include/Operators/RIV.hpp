#ifndef RIV_H_
#define RIV_H_

#include "IOperator.hpp"
#include <string>

/**
 * @class RIV
 * @brief A class used to compute the Variance/Reduction in Variance on a
 * dataset
 *
 * This class implements the IOperator interface
 */
class RIV : public IOperator {

private:
  const std::string name = "RIV"; /**< A string variable representing the name
                                of the class */

public:
  /**
   * @brief Default constructor for the RIV class.
   *
   * Initializes the class with default parameters.
   */
  RIV();

  /**
   * @brief Destructor for the RIV class
   */
  ~RIV() override;

  /**
   * @brief Prints the name of the class on standard output.
   */
  void print() override;

  /**
   * @brief Static methods that returns the name of the class (RIV)
   */
  static std::string get_Name();

  /**
   * @brief Computes the result of the RIV on the column at position folowwing
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
   * @brief Computes the Variance for a given data distribution.
   *
   * @param vector<double> prediction : The prediction we made
   * @param vector<size_t> idx : The real result
   *
   * @return Type: double, The RIV
   */
  static double apply(const std::vector<double> &exact,
                      const std::vector<double> &prediction);
};

#endif
