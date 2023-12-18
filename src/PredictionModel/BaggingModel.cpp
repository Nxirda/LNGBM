#include "BaggingModel.hpp"
#include "EnumOperator.hpp"

/********************/
/*                  */
/*  BAGGING CLASS   */
/*                  */
/********************/

/*
Default Constructor
Inputs  :
Outputs :
*/
BaggingModel::BaggingModel() {
  this->forest = RandomForest();
  this->max_Depth = 0;
  this->metric = "NO METRIC";
}

/*
Constructor with operator and depth fixed
Inputs  : string, int
Outputs :
*/
BaggingModel::BaggingModel(std::string split_Metric, int max_Depth) {
  // Here we just prepare the infos for the model
  this->metric = split_Metric;
  this->max_Depth = max_Depth;

  std::map<std::string, OperatorType>::iterator it;

  it = operator_Dictionnary.find(this->metric);

  if (it == operator_Dictionnary.end()) {
    std::cout << "Chosen metric is invalid \n" << std::endl;
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
    std::cout << "Chosen metric is invalid \n" << std::endl;
    abort();
  }
}

/*
Destructor
Inputs  :
Outputs :
*/
BaggingModel::~BaggingModel() { delete (this->split_Metric); };

/**/
std::string BaggingModel::get_Metric(){
  return this->metric;
}

/**/
void BaggingModel::train(const DataSet &data) { train(data, this->max_Depth); }

/*
train the model on the DataSet with the operator and depth fixed
Inputs  : const DataSet, int
Outputs :
*/
void BaggingModel::train(const DataSet &data, int n) {
  this->forest = RandomForest(data, this->split_Metric, n, this->max_Depth);

  this->forest.generate_Forest(n);
}

/*
predict the labels for the given dataset with the operator and depth fixed
Inputs  : const DataSet
Outputs :
*/
std::vector<float> BaggingModel::predict(const DataSet &data) {
  return this->forest.predict_Results(data);
}

/*
Prints the available operator for this library
Inputs  :
Outputs :
*/
void BaggingModel::print_Available_Operators() {
  for (auto const &pair : operator_Dictionnary) {
    std::cout << "{" << pair.first << "}\n";
  }
}