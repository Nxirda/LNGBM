#ifndef RANDOM_FOREST_H_
#define RANDOM_FOREST_H_

#include <map>
#include <vector>

#include "TrainingElement.hpp"
#include "DecisionTree.hpp"
#include "ICriteria.hpp"
#include "IOperator.hpp"

class RandomForest {

private:
  // Parameters

  int size;
  int max_Depth;
  IOperator *splitting_Operator;
  ICriterias *splitting_Criteria;
  std::map<int, DecisionTree> trees;
  DataSet dataset;

public:
  // Constructor

  RandomForest();
  RandomForest(const DataSet &dataset, IOperator *op, ICriterias *crit, int n, int depth);

  // Destructor

  ~RandomForest();

  // Getters

  int get_size();
  std::vector<float> get_results();

  // Methods
  void generate_Forest(int size);
  std::vector<float> predict_Results(const DataSet &dataset);
  void tree_Prediction(const DataSet &data,
                       std::shared_ptr<std::vector<float>> result,
                       std::vector<int> index, TreeNode *node);
};

#endif