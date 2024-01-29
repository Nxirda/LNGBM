#ifndef QUARTILES_H_
#define QUARTILES_H_

#include "ICriterias.hpp"
#include <string>

class Quartiles : public ICriterias {
private:
  // Parameters
  const std::string name = "Quartiles";
  const std::vector<float> quartiles = {25.0, 50.0, 75.0};

public:
  // Constructor
  Quartiles();

  // Destructor
  ~Quartiles() override;

  // Methods
  void print() override;
  std::vector<float> compute(const std::vector<float> column) const override;
};

#endif