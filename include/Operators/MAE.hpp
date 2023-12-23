#ifndef MAE_H_
#define MAE_H_

#include "IOperator.hpp"
#include <string>

class MAE : public IOperator {

private:
  // Parameters

  float split_Criterion;
  const std::string name = "MAE";

public:
  // Constructor

  MAE();

  // Destructor

  ~MAE() override;

  // Methods

  void print() override;

  float compute(int position, const DataSet &data,
                std::vector<int> index) const override;

  static double apply(const std::vector<float> &exact,
                     const std::vector<float> &prediction);
};

#endif