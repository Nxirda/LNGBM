#include "BaggingModel.hpp"
#include "EnumCriteria.hpp"
#include "EnumOperator.hpp"

/********************/
/*                  */
/*  BAGGING CLASS   */
/*                  */
/********************/

//
BaggingModel::BaggingModel(const std::string &split_Operator,
                           const std::string &split_Criteria, uint16_t max_Depth,
                           uint16_t number_Of_Trees) {

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

//
BaggingModel::~BaggingModel() {
  delete (this->split_Operator);
  delete (this->split_Criteria);
};

//
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

//
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
uint16_t BaggingModel::get_Depth() { return this->max_Depth; }

//
uint16_t BaggingModel::get_Trees_Number() { return this->forest.get_size(); };

//
const std::unordered_map<uint16_t, DecisionTree> &BaggingModel::get_Forest() {
  return this->forest.get_Trees();
}

//
void BaggingModel::train(const DataSet &data) {
  this->forest = RandomForest(this->split_Operator, this->split_Criteria,
                              this->number_Of_Trees, this->max_Depth);

  this->forest.train(data);
}

//
std::vector<double> BaggingModel::predict(const DataSet &data) const {
  return this->forest.predict(data);
}