#ifndef MAE_H_
#define MAE_H_

#include "IOperator.hpp"
#include <string>

class MAE : public IOperator {

private:
  // Parameters

  float split_Criteria;
  const std::string name = "MAE";
  std::shared_ptr<TreeNode> tree_Node;

public:
  // Constructor

  MAE(std::shared_ptr<TreeNode> tree_Node);

  // Destructor

  ~MAE() override;

  // Getter

  float get_Best_Split_Criteria() override;

  // Setter

  void set_Split_Criteria(float value) override;
  bool set_Node(std::shared_ptr<TreeNode> tree_Node);

  // Methods

  void print() override;

  int find_Best_Split_Feature() override;
  float splitting_MAE(int position);
};

#endif