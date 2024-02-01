#ifndef BAGGING_MODEL_H_
#define BAGGING_MODEL_H_

#include "DataSet.hpp"
#include "ICriteria.hpp"
#include "IOperator.hpp"
#include "RandomForest.hpp"

class BaggingModel {

private:
  // Parameters
  // Might wanna add a train/Test splitter for the dataset somewhere
  IOperator *split_Metric;
  ICriteria *split_Criteria;
  RandomForest forest;
  int max_Depth;

public:
  // Constructor

  BaggingModel() = delete;
  BaggingModel(std::string split_Metric, std::string split_Criteria,
               int max_Depth);

  // Getter
  
  int get_Depth();
  int get_Trees_Number();

  // Setters

  void set_Metric(std::string metric);
  void set_Criteria(std::string criteria);

  // Destructor

  ~BaggingModel();

  // Methods

  void train(const DataSet &data);
  void train(const DataSet &data, int n);
  std::vector<float> predict(const DataSet &datas);
};

#endif