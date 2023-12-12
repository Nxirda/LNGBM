#ifndef RMSE_H_
#define RMSE_H_

#include "IOperator.hpp"
#include <string>

class RMSE : public IOperator {

private:
  // Parameters

  float split_Criterion;
  const std::string name = "MAPE";
  // std::shared_ptr<TreeNode> tree_Node;

public:
  // Constructor
  RMSE();
  // RMSE(std::shared_ptr<TreeNode> tree_Node);

  // Destructor

  ~RMSE() override;

  // Getter

  // float get_Best_Split_Criterion() const override;

  // Setter

  // void set_Split_Criterion(float value) override;
  // bool set_Node(std::shared_ptr<TreeNode> tree_Node);

  // Methods

  void print() override;

  std::tuple<int, float> find_Best_Split(const DataSet &data,
                                         std::vector<int> index) const override;
  float splitting_RMSE(int position, const DataSet &data,
                      std::vector<int> index) const;
};

#endif