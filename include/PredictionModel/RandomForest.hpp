#ifndef RANDOM_FOREST_H_
#define RANDOM_FOREST_H_

#include <map>
#include <vector>

#include "DecisionTree.hpp"
#include "ICriteria.hpp"
#include "IOperator.hpp"
#include "TrainingElement.hpp"

class RandomForest {

private:
  // Parameters

  int size;
  int max_Depth;
  IOperator *splitting_Operator;
  ICriteria *splitting_Criteria;
  std::map<int, DecisionTree> trees;

public:
  // Constructor

  RandomForest();
  RandomForest(IOperator *op, ICriteria *crit, int n, int depth);

  // Destructor

  ~RandomForest();

  // Getters

  const std::map<int, DecisionTree> &get_Trees();
  int get_size();
  // std::vector<double> get_results();

  // Methods
  void generate_Forest(const DataSet &data, int size);
  void aggregate_Trees(const std::map<int, DecisionTree> &forest);
  std::vector<double> predict_Results(const DataSet &dataset);
  void tree_Prediction(const DataSet &data, std::vector<double> &result,
                       const std::vector<int> &index, TreeNode *node);
};

#endif