#ifndef MAPE_H_
#define MAPE_H_

#include "IOperator.hpp"
#include <string>

class MAPE : public IOperator {

private:
  // Parameters

  // double split_Criterion;
  const std::string name = "MAPE";

public:
  // Constructor

  MAPE();

  // Destructor

  ~MAPE() override;

  // Methods

  void print() override;

  static std::string get_Name();

  double compute(size_t position, const DataSet &data, const std::vector<size_t> &index,
                const double split_Criteria) const override;

  static double apply(const std::vector<double> &exact,
                      const std::vector<double> &prediction);
};

#endif