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
                           const std::string &split_Criteria,
                           uint16_t max_Depth, uint16_t number_Of_Trees) {

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
BaggingModel::~BaggingModel(){};

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
    this->split_Operator = std::make_unique<MAE>();
    break;
  case (operators::type::MAPE):
    this->split_Operator = std::make_unique<MAPE>();
    break;
  case (operators::type::RMSE):
    this->split_Operator = std::make_unique<RMSE>();
    break;
  case (operators::type::RIV):
    this->split_Operator = std::make_unique<RIV>();
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
    this->split_Criteria = std::make_unique<Histogram>();
    break;
  case (criterias::type::Percentiles):
    this->split_Criteria = std::make_unique<Percentiles>();
    break;
  case (criterias::type::Quartiles):
    this->split_Criteria = std::make_unique<Quartiles>();
    break;
  case (criterias::type::RandomValues):
    this->split_Criteria = std::make_unique<RandomValues>();
    break;
  case (criterias::type::UniqueValues):
    this->split_Criteria = std::make_unique<UniqueValues>();
    break;
  default:
    std::cerr << "Chosen criteria is invalid\n";
    abort();
  }
}

//
ICriteria *BaggingModel::get_Criteria() const {
  return this->split_Criteria.get();
}

//
IOperator *BaggingModel::get_Operator() const {
  return this->split_Operator.get();
}

//
uint16_t BaggingModel::get_Depth() const { return this->max_Depth; }

//
uint16_t BaggingModel::get_Trees_Number() const {
  return this->forest.get_size();
};

//
const std::unordered_map<uint16_t, DecisionTree> &
BaggingModel::get_Forest() const {
  return this->forest.get_Trees();
}

//
void BaggingModel::train(const DataSet &data) {
  ICriteria *criteria = this->split_Criteria.release();
  IOperator *op = this->split_Operator.release();

  train(data, criteria, op);

  this->split_Criteria.reset(criteria);
  this->split_Operator.reset(op);
}

//
void BaggingModel::train(const DataSet &data, ICriteria *crit, IOperator *op) {
  this->forest =
      std::move(RandomForest(this->number_Of_Trees, this->max_Depth));

  this->forest.train(data, crit, op);
}

//
std::vector<double> BaggingModel::predict(const DataSet &data) const {
  return this->forest.predict(data);
}