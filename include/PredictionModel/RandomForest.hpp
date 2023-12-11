#ifndef RANDOM_FOREST_H_
#define RANDOM_FOREST_H_

#include <map>
#include <vector>

#include "DecisionTree.hpp"
#include "IOperator.hpp"

class RandomForest {

private:
  int size;
  int max_Depth;
  IOperator *splitting_Operator;
  std::map<int, std::shared_ptr<DecisionTree>> trees;
  std::shared_ptr<DataSet> dataset;
  std::vector<float> results;

public:
  // Constructor

  RandomForest();
  RandomForest(std::shared_ptr<DataSet> dataset, IOperator *op, int n,
               int depth);

  // Destructor

  ~RandomForest();

  // Getters

  int get_size();
  std::vector<float> get_results();
  std::shared_ptr<DataSet> get_Dataset();

  // Setters

  // Methods
  void generate_Forest(int size);
  void predict_Results(std::shared_ptr<DataSet> dataset);
};

#endif