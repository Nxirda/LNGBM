#ifndef RANDOM_VALUES_H_
#define RANDOM_VALUES_H_

#include "ICriteria.hpp"
#include <string>

class Random_Values : public ICriterias {
private:
  // Parameters
  const std::string name = "Random_Values";
  int number_Of_Elements = 10;

public:
  // Constructor
  Random_Values();

  Random_Values(int x);

  // Destructor
  ~Random_Values() override;

  // Methods
  void print() override;

  std::vector<float> compute(const std::vector<float> list) const override;

  float get_Random_Float(float min , float max) const;
};

#endif