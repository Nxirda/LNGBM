#ifndef RMSE_H_
#define RMSE_H_

#include "IOperator.hpp"
#include <string>

class RMSE : public IOperator {

private:
  // Parameters

  float split_Criterion;
  const std::string name = "MAPE";

public:
  // Constructor

  RMSE();

  // Destructor

  ~RMSE() override;

  // Methods

  void print() override;

  float compute(int position, const DataSet &data,
                std::vector<int> index) const override;
};

#endif