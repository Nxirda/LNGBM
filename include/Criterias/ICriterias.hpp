#ifndef I_CRITERIAS_H_
#define I_CRITERIAS_H_

#include <memory>
#include <string>

#include "DataSet.hpp"

// Interface to build Operators on
class ICriterias {

private:
  const std::string name;

public:
  virtual ~ICriterias(){};

  //virtual std::vector<float> compute(const std::vector<float> column) const = 0;

  virtual std::vector<float> compute(const std::vector<float> column) const = 0;

  virtual void print() = 0;
};

#endif