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
  int max_Depth;
  int number_Of_Trees;

public:
  // Constructor

  BaggingModel() = delete;
  BaggingModel(const std::string &split_Operator,
               const std::string &split_Criteria, int max_Depth,
               int number_Of_Trees);

  // Getter

  int get_Depth();
  int get_Trees_Number();
  const std::unordered_map<int, DecisionTree> &get_Forest();

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