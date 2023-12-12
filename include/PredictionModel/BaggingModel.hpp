#ifndef BAGGING_MODEL_H_
#define BAGGING_MODEL_H_

#include "DataSet.hpp"
#include "IOperator.hpp"
#include "RandomForest.hpp"

class BaggingModel {

private:
  // Parameters
  // Might wanna add a train/Test splitter for the dataset somewhere
  IOperator *split_Metric;
  RandomForest forest;

  int max_Depth;
  std::string metric;

public:
  // Constructor

  BaggingModel();
  BaggingModel(std::string split_Metric, int max_Depth);

  // Destructor

  ~BaggingModel();

  // Methods

  void train(const DataSet &data, int n);
  std::vector<float> predict(const DataSet &datas);
  void print_Available_Operators();

};

#endif