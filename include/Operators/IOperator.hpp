#ifndef I_OPERATOR_H_
#define I_OPERATOR_H_

#include <memory>
#include <string>

#include "DataSet.hpp"

// Interface to build Operators on
class IOperator {

private:
  const std::string name;
  // std::shared_ptr<TreeNode> tree_Node;
  //float split_Criterion;

public:
  virtual ~IOperator(){};

  virtual std::tuple<int, float> find_Best_Split(const DataSet &data, std::vector<int> index) const = 0;
  //virtual float get_Best_Split_Criterion() const = 0;
  //virtual void set_Split_Criterion(float) = 0;

  virtual void print() = 0;
};

#endif