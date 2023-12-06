#ifndef RIV_H_
#define RIV_H_

#include "IOperator.hpp"
#include <string>

class ReductionInVar : public IOperator {

private:
  // Parameters

  float split_Criteria;
  const std::string name = "RIV";
  std::shared_ptr<TreeNode> tree_Node;

public:
  // Constructor

  ReductionInVar(std::shared_ptr<TreeNode> tree_Node);

  // Destructor

  ~ReductionInVar() override;

  // Getter

  float get_Best_Split_Criteria() override;

  // Setter

  void set_Split_Criteria(float value) override;
  bool set_Node(std::shared_ptr<TreeNode> tree_Node);

  // Methods

  void print() override;

  int find_Best_Split_Feature() override;
  float splitting_Variance(int position);
};

#endif