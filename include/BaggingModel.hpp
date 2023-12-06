#ifndef BAGGING_MODEL_H_
#define BAGGING_MODEL_H_

#include "DataSet.hpp"
#include "DecisionTree.hpp"
#include "IOperator.hpp"

class BaggingModel {

private:
  // Parameters
  // Might wanna add a train/Test splitter for the dataset somewhere
  IOperator *split_Metric;
  std::unique_ptr<DecisionTree> main_Tree;

  int max_Depth;
  std::string metric;

public:
  // Constructor

  BaggingModel();
  BaggingModel(std::string split_Metric, int max_Depth);

  // Destructor

  ~BaggingModel();

  // Getters

  std::vector<float> get_Prediction();

  // Methods

  // void fit(const DataSet &data);
  void train(const DataSet &data, int n);
  void predict(const DataSet &datas);
  void print_Available_Operators();

  std::vector<int> bootstrap_DataSet();
};

#endif