#ifndef I_OPERATOR_H_
#define I_OPERATOR_H_

#include <string>
#include <memory>

class DecisionTree;

// Interface to build Operators on
class IOperator {

private:
  const std::string name;
  std::unique_ptr<DecisionTree> tree;
  float split_Criteria;

public:
  virtual ~IOperator(){};
  virtual bool set_Tree(DecisionTree *tree) {
    if (tree)
      return true;
    return false;
  };
  virtual void set_Split_Criteria(float value) {
    split_Criteria = value;
  }
  virtual void print(){};
  virtual int find_Best_Split_Feature() { return 0; };
  virtual float get_Best_Split_Criteria() { return 0.0; };
};

#endif