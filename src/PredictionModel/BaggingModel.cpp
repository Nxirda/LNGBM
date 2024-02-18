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
BaggingModel::BaggingModel(const std::string &split_Metric, const std::string &split_Criteria,
                           int max_Depth) {

  // Here we just prepare the infos for the model

  if (max_Depth < 1) {
    std::cerr << "Depth parameter should be at least 1\n";
    abort();
  }

  this->max_Depth = max_Depth;
  set_Metric(split_Metric);
  set_Criteria(split_Criteria);
}

/*
Destructor
Parameters :
Inputs     :
Outputs    :
*/
BaggingModel::~BaggingModel() {
  delete (this->split_Metric);
  delete (this->split_Criteria);
};

/*
Sets the metric to split the nodes of the trees
Parameters : split metric
Inputs     : string
Outputs    :
*/
void BaggingModel::set_Metric(const std::string &metric) {

  std::map<std::string, operators::type>::iterator it;

  it = operators::dictionnary.find(metric);

  if (it == operators::dictionnary.end()) {
    std::cerr << "Chosen metric is invalid\n";
    abort();
  }

  switch (it->second) {
  case (operators::type::MAE):
    this->split_Metric = new MAE();
    break;
  case (operators::type::MAPE):
    this->split_Metric = new MAPE();
    break;
  case (operators::type::RMSE):
    this->split_Metric = new RMSE();
    break;
  case (operators::type::RIV):
    this->split_Metric = new RIV();
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

/*
 */
int BaggingModel::get_Depth() { return this->max_Depth; }

/*
 */
int BaggingModel::get_Trees_Number() { return this->forest.get_size(); };

/**/
const std::map<int, DecisionTree> &BaggingModel::get_Forest() {
  return this->forest.get_Trees();
}

/*
 */
void BaggingModel::aggregate_Forest(const std::map<int, DecisionTree> &forest) {
  this->forest.aggregate_Trees(forest);
}

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
    /* errno = EINVAL;
    perror("Number of Trees should be at least 1");
    abort(); */
    std::cerr << "Number of Trees should be at least 1\n";
    abort();
    
  }

  this->forest = RandomForest(this->split_Metric, this->split_Criteria, n,
                              this->max_Depth);

  this->forest.generate_Forest(data, n);
}

/*
predict the labels for the given dataset with the operator and depth fixed
Parameters : Dataset for prediction
Inputs     : const DataSet
Outputs    : vector<double>
*/
std::vector<double> BaggingModel::predict(const DataSet &data) {
  return this->forest.predict_Results(data);
}