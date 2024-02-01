#ifndef RIV_H_
#define RIV_H_

#include "IOperator.hpp"
#include <string>

class RIV : public IOperator {

private:
  // Parameters

  // float split_Criterion;
  const std::string name = "RIV";

public:
  // Constructor

  RIV();

  // Destructor

  ~RIV() override;

  // Methods

  void print() override;

  static std::string get_Name();

  float compute(int position, const DataSet &data, std::vector<int> index,
                const float split_Criteria) const override;

  static double apply(const std::vector<float> &exact,
                      const std::vector<float> &prediction);
};

#endif