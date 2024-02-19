#ifndef RANDOM_FOREST_H_
#define RANDOM_FOREST_H_

#include <unordered_map>
#include <vector>

#include "IModel.hpp"
#include "DecisionTree.hpp"

class RandomForest : IModel {

private:
  // Parameters

  int size;
  int max_Depth;
  IOperator *splitting_Operator;
  ICriteria *splitting_Criteria;
  std::unordered_map<int, DecisionTree> trees;

public:
  // Constructor

  RandomForest();
  RandomForest(IOperator *op, ICriteria *crit, int n, int depth);

  // Destructor

  ~RandomForest() override;

  // Getters

  const std::unordered_map<int, DecisionTree> &get_Trees();
  int get_size();

  // Methods

  void train(const DataSet &data) override;
  std::vector<double> predict(const DataSet &data) const override;
};

#endif