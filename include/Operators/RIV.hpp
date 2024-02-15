#ifndef RIV_H_
#define RIV_H_

#include "IOperator.hpp"
#include <string>

class RIV : public IOperator {

private:
  // Parameters

  // double split_Criterion;
  const std::string name = "RIV";

public:
  // Constructor

  RIV();

  // Destructor

  ~RIV() override;

  // Methods

  void print() override;

  static std::string get_Name();

  double compute(int position, const DataSet &data, const std::vector<int> &index,
                const double split_Criteria) const override;

  static double apply(const std::vector<double> &exact,
                      const std::vector<double> &prediction);
};

#endif