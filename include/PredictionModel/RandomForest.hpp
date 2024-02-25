#ifndef RANDOM_FOREST_H_
#define RANDOM_FOREST_H_

#include <unordered_map>
#include <vector>

#include "IModel.hpp"
#include "DecisionTree.hpp"

class RandomForest : IModel {

private:
  // Parameters

  uint16_t size;
  uint16_t max_Depth;
  IOperator *splitting_Operator;
  ICriteria *splitting_Criteria;
  std::unordered_map<uint16_t, DecisionTree> trees;

public:
  // Constructor

  RandomForest();
  RandomForest(IOperator *op, ICriteria *crit, uint16_t n, uint16_t depth);

  // Destructor

  ~RandomForest() override;

  // Getters

  const std::unordered_map<uint16_t, DecisionTree> &get_Trees();
  uint16_t get_size();

  // Methods

  void train(const DataSet &data) override;
  std::vector<double> predict(const DataSet &data) const override;
};

#endif