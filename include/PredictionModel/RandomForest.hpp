#ifndef RANDOM_FOREST_H_
#define RANDOM_FOREST_H_

#include <unordered_map>
#include <vector>

#include "DecisionTree.hpp"
#include "IModel.hpp"

class RandomForest : IModel {

private:
  // Parameters

  uint16_t size;
  uint16_t max_Depth;

  std::unordered_map<uint16_t, DecisionTree> trees;

public:
  // Constructor

  RandomForest();
  RandomForest(uint16_t n, uint16_t depth);
  // Destructor

  ~RandomForest() override;

  // Getters
  const std::unordered_map<uint16_t, DecisionTree> &get_Trees() const;
  uint16_t get_size() const;

  // Methods

  void train(const DataSet &data, ICriteria *crit, IOperator *op) override;
  std::vector<double> predict(const DataSet &data) const override;
};

#endif