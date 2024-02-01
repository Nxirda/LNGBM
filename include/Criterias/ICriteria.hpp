#ifndef I_CRITERIA_H_
#define I_CRITERIA_H_

#include <memory>
#include <string>

#include "DataSet.hpp"

// Interface to build Operators on
class ICriteria {

private:
  const std::string name;

public:

  virtual ~ICriteria(){};
  
  virtual std::vector<float> compute(const std::vector<float> list) const = 0;

  virtual void print() = 0;
};

#endif