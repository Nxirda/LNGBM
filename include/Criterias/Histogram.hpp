#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "ICriterias.hpp"
#include <string>

class Histogram : public ICriterias {
private:
  // Parameters
  const std::string name = "Histogram";
  int number_Of_Bins = 10;

public:
  // Constructor
  Histogram();

  Histogram(int x);

  // Destructor
  ~Histogram() override;

  // Methods
  void print() override;

  std::vector<float> compute(const std::vector<float> column) const override;
};

#endif