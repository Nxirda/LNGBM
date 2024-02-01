#ifndef RANDOM_VALUES_H_
#define RANDOM_VALUES_H_

#include "ICriteria.hpp"
#include <string>

class RandomValues : public ICriteria {
private:
  // Parameters
  const std::string name = "Random_Values";
  int number_Of_Elements = 10;

public:
  // Constructor
  RandomValues();

  RandomValues(int x);

  // Destructor
  ~RandomValues() override;

  static std::string get_Name();

  // Methods
  void print() override;
    static void printf();


  std::vector<float> compute(const std::vector<float> list) const override;

  float get_Random_Float(float min , float max) const;
};

#endif