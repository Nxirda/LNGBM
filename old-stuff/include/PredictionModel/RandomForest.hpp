#ifndef RANDOM_FOREST_H_
#define RANDOM_FOREST_H_

#include <unordered_map>
#include <vector>

#include "DecisionTree.hpp"
#include "IModel.hpp"

class RandomForest : IModel {

private:
  // Parameters
  
  std::unordered_map<uint16_t, DecisionTree> trees;
  uint16_t size;
  uint16_t max_Depth;

public:
  // Constructor

  RandomForest() noexcept;
  RandomForest(uint16_t n, uint16_t depth) noexcept;

  RandomForest(RandomForest &&forest) noexcept;
  RandomForest &operator=(RandomForest &&forest) noexcept;
  RandomForest &operator=(const RandomForest &forest) noexcept;
  // Destructor

  ~RandomForest() override;

  // Getters
  const std::unordered_map<uint16_t, DecisionTree> &get_Trees() const;
  uint16_t get_size() const;

  // Methods
  /* void train(const DataSet &data) override; */
  void train(const DataSet &data, uint64_t bins) override;
  void train(const DataSet &data, ICriteria *crit, IOperator *op) override;
  std::vector<double> predict(const DataSet &data) const override;
};

#endif