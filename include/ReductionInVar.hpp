#ifndef RIV_H_
#define RIV_H_

#include "IOperator.hpp"
#include <string>

class ReductionInVar : public IOperator {

private:
  // Parameters

  const std::string name = "RIV";
  DecisionTree *tree;
  float split_Criteria;

public:
  // Constructor

  ReductionInVar(DecisionTree *tree);

  // Destructor

  ~ReductionInVar() override;

  // Getter

  float get_Best_Split_Criteria() override;

  // Setter

  bool set_Tree(DecisionTree *tree) override;
  void set_Split_Criteria(float value) override;

  // Methods

  void print() override;
  int find_Best_Split_Feature() override;

  float splitting_Variance(int position);
};

#endif