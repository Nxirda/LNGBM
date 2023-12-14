#ifndef I_OPERATOR_H_
#define I_OPERATOR_H_

#include <memory>
#include <string>

#include "DataSet.hpp"

// Interface to build Operators on
class IOperator {

private:
  const std::string name;

public:
  virtual ~IOperator(){};

  virtual float compute(int position, const DataSet &data,
                        std::vector<int> index) const = 0;

  virtual void print() = 0;
};

#endif