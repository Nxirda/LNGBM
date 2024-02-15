#ifndef MAE_H_
#define MAE_H_

#include "IOperator.hpp"
#include <string>

class MAE : public IOperator {

private:
  // Parameters

  //double split_Criterion;
  const std::string name = "MAE";

public:
  // Constructor

  MAE();

  // Destructor

  ~MAE() override;

  // Methods

  void print() override;

  static std::string get_Name();

  double compute(int position, const DataSet &data, const std::vector<int> &index,
                const double split_Criteria) const override;

  static double apply(const std::vector<double> &exact,
                      const std::vector<double> &prediction);
};

#endif