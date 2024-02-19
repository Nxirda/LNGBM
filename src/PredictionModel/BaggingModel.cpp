#include "BaggingModel.hpp"
#include "EnumCriteria.hpp"
#include "EnumOperator.hpp"

/********************/
/*                  */
/*  BAGGING CLASS   */
/*                  */
/********************/

/*
Constructor with operator and depth fixed
Parameters : split metric, split criteria, max depth
Inputs     : string, string, int
Outputs    : Object of BaggingModel class
*/
BaggingModel::BaggingModel(const std::string &split_Operator,
                           const std::string &split_Criteria, int max_Depth,
                           int number_Of_Trees) {

  // Here we just prepare the infos for the model

  if (max_Depth < 1) {
    std::cerr << "Depth parameter should be at least 1\n";
    abort();
  }
  this->number_Of_Trees = number_Of_Trees;
  this->max_Depth = max_Depth;
  set_Operator(split_Operator);
  set_Criteria(split_Criteria);
}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
BaggingModel::~BaggingModel() {
  delete (this->split_Operator);
  delete (this->split_Criteria);
};

/*
Sets the metric to split the nodes of the trees
Parameters : split metric
Inputs     : string
Outputs    :
*/
void BaggingModel::set_Operator(const std::string &metric) {

  std::map<std::string, operators::type>::iterator it;

  it = operators::dictionnary.find(metric);

  if (it == operators::dictionnary.end()) {
    std::cerr << "Chosen metric is invalid\n";
    abort();
  }

  switch (it->second) {
  case (operators::type::MAE):
    this->split_Operator = new MAE();
    break;
  case (operators::type::MAPE):
    this->split_Operator = new MAPE();
    break;
  case (operators::type::RMSE):
    this->split_Operator = new RMSE();
    break;
  case (operators::type::RIV):
    this->split_Operator = new RIV();
    break;
  default:
    std::cerr << "Chosen metric is invalid\n";
    abort();
  }
}

/*
Sets the criteria to split the nodes of the trees
Parameters : split criteria
Inputs     : string
Outputs    :
*/
void BaggingModel::set_Criteria(const std::string &criteria) {

  std::map<std::string, criterias::type>::iterator it;

  it = criterias::dictionary.find(criteria);

  if (it == criterias::dictionary.end()) {
    std::cerr << "Chosen criteria is invalid\n";
    abort();
  }

  switch (it->second) {
  case (criterias::type::Histogram):
    this->split_Criteria = new Histogram();
    break;
  case (criterias::type::Percentiles):
    this->split_Criteria = new Percentiles();
    break;
  case (criterias::type::Quartiles):
    this->split_Criteria = new Quartiles();
    break;
  case (criterias::type::RandomValues):
    this->split_Criteria = new RandomValues();
    break;
  case (criterias::type::UniqueValues):
    this->split_Criteria = new UniqueValues();
    break;
  default:
    std::cerr << "Chosen criteria is invalid\n";
    abort();
  }
}

//
int BaggingModel::get_Depth() { return this->max_Depth; }

//
int BaggingModel::get_Trees_Number() { return this->forest.get_size(); };

//
const std::unordered_map<int, DecisionTree> &BaggingModel::get_Forest() {
  return this->forest.get_Trees();
}

/*
Train the trees on the given dataset on the configured max depth
Parameters : Dataset for training
Inputs     : const DataSet
Outputs    :
*/
void BaggingModel::train(const DataSet &data) {
  this->forest = RandomForest(this->split_Operator, this->split_Criteria,
                              this->number_Of_Trees, this->max_Depth);

  this->forest.train(data);
}

/*
predict the labels for the given dataset with the operator and depth fixed
Parameters : Dataset for prediction
Inputs     : const DataSet
Outputs    : vector<double>
*/
std::vector<double> BaggingModel::predict(const DataSet &data) const {
  return this->forest.predict(data);
}