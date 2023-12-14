#ifndef RIV_H_
#define RIV_H_

#include "IOperator.hpp"
#include <string>

class ReductionInVar : public IOperator {

private:
  // Parameters

  float split_Criterion;
  const std::string name = "RIV";

public:
  // Constructor

  ReductionInVar();

  // Destructor

  ~ReductionInVar() override;

  // Methods

  void print() override;

  float compute(int position, const DataSet &data,
                std::vector<int> index) const override;
};

#endif