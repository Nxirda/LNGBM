#ifndef MAPE_H_
#define MAPE_H_

#include "IOperator.hpp"
#include <string>

class MAPE : public IOperator {

private:
  // Parameters

  float split_Criteria;
  const std::string name = "MAPE";
  std::shared_ptr<TreeNode> tree_Node;

public:
  // Constructor

  MAPE(std::shared_ptr<TreeNode> tree_Node);

  // Destructor

  ~MAPE() override;

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