#ifndef UNIQUE_VALUES_H_
#define UNIQUE_VALUES_H_

#include "ICriteria.hpp"
#include <string>

class UniqueValues : public ICriteria {
private:
  // Parameters
  const std::string name = "Unique_Values";

public:
  // Constructor
  UniqueValues();

  // Destructor
  ~UniqueValues() override;

  // Methods
  static std::string get_Name();

  void print() override;

  std::vector<float> compute(const std::vector<float> list) const override;
};

#endif