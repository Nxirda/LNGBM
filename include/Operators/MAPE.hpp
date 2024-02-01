#ifndef MAPE_H_
#define MAPE_H_

#include "IOperator.hpp"
#include <string>

class MAPE : public IOperator {

private:
  // Parameters

  // float split_Criterion;
  const std::string name = "MAPE";

public:
  // Constructor

  MAPE();

  // Destructor

  ~MAPE() override;

  // Methods

  void print() override;

  static std::string get_Name();

  float compute(int position, const DataSet &data, std::vector<int> index,
                const float split_Criteria) const override;

  static float apply(const std::vector<float> &exact,
                      const std::vector<float> &prediction);
};

#endif