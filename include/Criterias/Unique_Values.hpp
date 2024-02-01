#ifndef UNIQUE_VALUES_H_
#define UNIQUE_VALUES_H_

#include "ICriterias.hpp"
#include <string>

class Unique_Values : public ICriterias {
private:
  // Parameters
  const std::string name = "Unique_Values";

public:
  // Constructor
  Unique_Values();

  // Destructor
  ~Unique_Values() override;

  // Methods
  void print() override;

  std::vector<float> compute(const std::vector<float> list) const override;
};

#endif