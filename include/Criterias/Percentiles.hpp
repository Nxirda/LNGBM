#ifndef PERCENTILES_H_
#define PERCENTILES_H_

#include "ICriteria.hpp"
#include <string>

class Percentiles : public ICriterias {
private:
  // Parameters
  const std::string name = "Percentiles";
  const std::vector<float> percentiles = {10.0, 20.0, 30.0, 40.0, 50.0,
                                          60.0, 70.0, 80.0, 90.0};

public:
  // Constructor
  Percentiles();

  // Destructor
  ~Percentiles() override;

  // Methods
  void print() override;

  std::vector<float> compute(const std::vector<float> list) const override;
};

#endif