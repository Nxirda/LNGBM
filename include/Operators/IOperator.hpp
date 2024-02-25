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

  virtual double compute(size_t position, const DataSet &data,
                        const std::vector<size_t> &index, const double criteria) const = 0;

  virtual void print() = 0;
};

#endif