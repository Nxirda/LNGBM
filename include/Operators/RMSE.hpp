#ifndef RMSE_H_
#define RMSE_H_

#include "IOperator.hpp"
#include <string>

class RMSE : public IOperator {

private:
  // Parameters

  // double split_Criterion;
  const std::string name = "MAPE";

public:
  // Constructor

  RMSE();

  // Destructor

  ~RMSE() override;

  // Methods

  void print() override;

  static std::string get_Name();

  double compute(size_t position, const DataSet &data, const std::vector<size_t> &index,
                const double split_Criteria) const override;

};

#endif