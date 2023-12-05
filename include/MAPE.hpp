#ifndef MAPE_H_
#define MAPE_H_

#include "IOperator.hpp"
#include <string>

class MAPE : public IOperator {

private:
  // Parameters

  const std::string name = "MAPE";
  std::shared_ptr<TreeNode> tree_Node;
  float split_Criteria;

public:
  // Constructor

  MAPE(std::shared_ptr<TreeNode> tree_Node);

  // Destructor

  ~MAPE() override;

  // Getter

  float get_Best_Split_Criteria() override;

  // Setter

  bool set_Node(std::shared_ptr<TreeNode> tree_Node);
  void set_Split_Criteria(float value) override;

  // Methods

  void print() override;
  int find_Best_Split_Feature() override;

  float splitting_MAE(int position);
};

#endif