#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

#include "ICriteria.hpp"
#include <string>

class Histogram : public ICriteria{
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

  static std::string get_Name();

  std::vector<float> compute(const std::vector<float> list) const override;
};

#endif