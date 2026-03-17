#include "BaggingModel.hpp"
#include "EnumCriteria.hpp"
#include "EnumOperator.hpp"

/********************/
/*                  */
/*  BAGGING CLASS   */
/*                  */
/********************/

//
BaggingModel::BaggingModel() noexcept {};

//
BaggingModel::BaggingModel(const std::string &split_Operator,
                           const std::string &split_Criteria,
                           uint16_t max_Depth, uint16_t trees_Number) noexcept {

  // Here we just prepare the infos for the model
  if (max_Depth < 1) {
    std::cerr << " < Depth parameter should be at least 1\n";
    exit(1);
  }
  this->number_Of_Trees = trees_Number;
  this->max_Depth = max_Depth;
  set_Operator(split_Operator);
  set_Criteria(split_Criteria);
  this->bins = 0;
}

//
BaggingModel::BaggingModel(uint64_t bins, uint16_t depth,
                           uint16_t number_Of_Trees) noexcept {
  // Here we just prepare the infos for the model
  if (depth < 1) {
    std::cerr << " < Depth parameter should be at least 1\n";
    exit(1);
  }
  this->number_Of_Trees = number_Of_Trees;
  this->max_Depth = depth;
  this->bins = bins;
  this->split_Operator = nullptr;
  this->split_Criteria = nullptr;
}

//
BaggingModel::BaggingModel(BaggingModel &&model) noexcept {
  this->forest = std::move(model.forest);
  this->max_Depth = model.max_Depth;
  this->number_Of_Trees = model.number_Of_Trees;
  if (model.split_Criteria != nullptr) {
    this->split_Criteria = std::move(model.split_Criteria);
  }
  if (model.split_Operator != nullptr) {
    this->split_Operator = std::move(model.split_Operator);
  }

  /* this->split_Criteria = std::move(model.split_Criteria);
  this->split_Operator = std::move(model.split_Operator); */
  this->bins = model.bins;
}

//
BaggingModel &BaggingModel::operator=(const BaggingModel &model) noexcept {
  this->forest = model.forest;
  this->max_Depth = model.max_Depth;
  this->number_Of_Trees = model.number_Of_Trees;

  if (model.split_Criteria != nullptr) {
    this->set_Criteria(model.get_Criteria()->get_Name());
  }
  if (model.split_Operator != nullptr) {
    this->set_Operator(model.get_Operator()->get_Name());
  }

  this->bins = model.bins;
  return *this;
}

//
BaggingModel &BaggingModel::operator=(BaggingModel &&model) noexcept {
  this->forest = std::move(model.forest);
  this->max_Depth = model.max_Depth;
  this->number_Of_Trees = model.number_Of_Trees;
  if (model.split_Criteria != nullptr) {
    this->set_Criteria(model.get_Criteria()->get_Name());
  }
  if (model.split_Operator != nullptr) {
    this->set_Operator(model.get_Operator()->get_Name());
  }
  this->bins = model.bins;
  return *this;
}

//
BaggingModel::~BaggingModel(){};

//
void BaggingModel::set_Operator(const std::string &metric) {

  std::map<std::string, operators::type>::iterator it;

  it = operators::dictionnary.find(metric);

  if (it == operators::dictionnary.end()) {
    std::cerr << " < Chosen metric is invalid : " << metric << "\n";
    exit(1);
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
    std::cerr << " < Chosen metric is invalid : " << metric << "\n";
    exit(1);
  }
}

//
void BaggingModel::set_Criteria(const std::string &criteria) {

  std::map<std::string, criterias::type>::iterator it;

  it = criterias::dictionary.find(criteria);

  if (it == criterias::dictionary.end()) {
    std::cerr << " < Chosen criteria is invalid : " << criteria << "\n";
    exit(1);
  }

  switch (it->second) {
  case (criterias::type::UniformDistribution):
    this->split_Criteria = std::make_unique<UniformDistribution>();
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
    std::cerr << " < Chosen criteria is invalid : " << criteria << "\n";
    exit(1);
  }
}

//
void BaggingModel::set_Depth(uint16_t depth) { this->max_Depth = depth; }

//
void BaggingModel::set_Trees_Number(uint16_t trees) {
  this->number_Of_Trees = trees;
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
  return this->number_Of_Trees;
};

//
const std::unordered_map<uint16_t, DecisionTree> &
BaggingModel::get_Forest() const {
  return this->forest.get_Trees();
}

// What the fuck ??????? dont release & shit
void BaggingModel::train(const DataSet &data) {
  // Depending on the mode used
  if (this->split_Criteria != nullptr && this->split_Operator != nullptr) {
    /* ICriteria *criteria = this->split_Criteria.release();
    IOperator *op = this->split_Operator.release(); */
    
    train(data, this->split_Criteria.get(), this->split_Operator.get());

    /* this->split_Criteria.reset(criteria);
    this->split_Operator.reset(op); */
  } else {
    train(data, this->bins);
  }
}

//
void BaggingModel::train(const DataSet &data, ICriteria *crit, IOperator *op) {
  this->forest =
      std::move(RandomForest(this->number_Of_Trees, this->max_Depth));

  this->forest.train(data, crit, op);
}

//
void BaggingModel::train(const DataSet &data, uint64_t bins) {
  this->forest =
      std::move(RandomForest(this->number_Of_Trees, this->max_Depth));

  this->forest.train(data, bins);
}

//
std::vector<double> BaggingModel::predict(const DataSet &data) const {
  return this->forest.predict(data);
}