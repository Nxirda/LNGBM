#ifndef BAGGING_MODEL_H_
#define BAGGING_MODEL_H_

#include "IModel.hpp"
#include "RandomForest.hpp"

class BaggingModel : IModel {

private:
  // Parameters
  IOperator *split_Operator;
  ICriteria *split_Criteria;
  RandomForest forest;
  uint16_t max_Depth;
  uint16_t number_Of_Trees;

public:
  // Constructor

  BaggingModel() = delete;
  BaggingModel(const std::string &split_Operator,
               const std::string &split_Criteria, uint16_t max_Depth,
               uint16_t number_Of_Trees);

  // Getter

  uint16_t get_Depth();
  uint16_t get_Trees_Number();
  const std::unordered_map<uint16_t, DecisionTree> &get_Forest();

  // Setters

  void set_Operator(const std::string &op);
  void set_Criteria(const std::string &criteria);

  // Destructor

  ~BaggingModel() override;

  // Methods
  void train(const DataSet &data) override;
  std::vector<double> predict(const DataSet &datas) const override;
};

#endif