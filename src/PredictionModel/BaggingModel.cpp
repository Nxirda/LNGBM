#include <errno.h>

#include "BaggingModel.hpp"
#include "EnumOperator.hpp"

/********************/
/*                  */
/*  BAGGING CLASS   */
/*                  */
/********************/

/*
Default Constructor
Parameters :
Inputs     :
Outputs    :
*/
BaggingModel::BaggingModel() {
  this->forest = RandomForest();
  this->max_Depth = 0;
  this->metric = "NO METRIC";
  this->split_Metric = nullptr;
}

/*
Constructor with operator and depth fixed
Parameters : split metric, max depth
Inputs     : string, int
Outputs    :
*/
BaggingModel::BaggingModel(std::string split_Metric, int max_Depth) {

  // Here we just prepare the infos for the model

  if (max_Depth < 1) {
    errno = EINVAL;
    perror("Depth parameter should be at least 1");
    abort();
  }

  this->metric = split_Metric;
  this->max_Depth = max_Depth;

  std::map<std::string, OperatorType>::iterator it;

  it = operator_Dictionnary.find(this->metric);

  if (it == operator_Dictionnary.end()) {
    errno = EINVAL;
    perror("Chosen metric is invalid");
    abort();
  }

  switch (it->second) {
  case (OperatorType::MAE):
    this->split_Metric = new MAE();
    break;
  case (OperatorType::MAPE):
    this->split_Metric = new MAPE();
    break;
  case (OperatorType::RMSE):
    this->split_Metric = new RMSE();
    break;
  case (OperatorType::ReductionInVar):
    this->split_Metric = new ReductionInVar();
    break;
  default:
    errno = EINVAL;
    perror("Chosen metric is invalid");
    abort();
  }
}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
BaggingModel::~BaggingModel() { delete (this->split_Metric); };

/*
Returns the actual metric used for splitting
Parameters :
Inputs     :
Outputs    : string
*/
std::string BaggingModel::get_Metric() { return this->metric; }

/*
Train the trees on the given dataset on the configured max depth
Parameters : Dataset for training
Inputs     : const DataSet
Outputs    :
*/
void BaggingModel::train(const DataSet &data) {
  this->train(data, this->forest.get_size());
}

/*
train the model on the DataSet with the operator and depth fixed
Parameters : Dataset for training, number of trees to train
Inputs     : const DataSet, int
Outputs    :
*/
void BaggingModel::train(const DataSet &data, int n) {

  if (n < 1) {
    errno = EINVAL;
    perror("Number of Trees should be at least 1");
    abort();
  }

  this->forest = RandomForest(data, this->split_Metric, n, this->max_Depth);

  this->forest.generate_Forest(n);
}

/*
predict the labels for the given dataset with the operator and depth fixed
Parameters : Dataset for prediction
Inputs     : const DataSet
Outputs    : vector<float>
*/
std::vector<float> BaggingModel::predict(const DataSet &data) {
  return this->forest.predict_Results(data);
}

/*
Prints the available operator for this library
Parameters :
Inputs     :
Outputs    :
*/
void BaggingModel::print_Available_Operators() {
  for (auto const &pair : operator_Dictionnary) {
    std::cout << "{" << pair.first << "}\n";
  }
  return;
}