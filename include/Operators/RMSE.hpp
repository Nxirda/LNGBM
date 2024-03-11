#ifndef RMSE_H_
#define RMSE_H_

#include "IOperator.hpp"
#include <string>

/**
 * @class RMSE
 * @brief A class used to compute the Root Mean Square Error on a
 * dataset
 *
 * This class implements the IOperator interface
 */
class RMSE : public IOperator {

private:
  const std::string name = "MAPE"; /**< A string variable representing the name
                               of the class */

public:
  /**
   * @brief Default constructor for the RMSE class.
   *
   * Initializes the class with default parameters.
   */
  RMSE();

  /**
   * @brief Destructor for the RMSE class
   */
  ~RMSE() override;

  /**
   * @brief Prints the name of the class on standard output.
   */
  void print() override;

  /**
   * @brief Methods that returns the name of the class (RMSE)
   */
  std::string get_Name() const override;

  /**
   * @brief Static methods that returns the name of the class (RMSE)
   */
  static std::string get_Name_Static();

  /**
   * @brief Computes the result of the RMSE on the column at position folowwing
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
};

#endif