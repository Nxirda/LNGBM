#ifndef BAGGING_MODEL_H_
#define BAGGING_MODEL_H_

//#include "DataSet.hpp"
#include "IModel.hpp"
#include "ICriteria.hpp"
#include "IOperator.hpp"
#include "RandomForest.hpp"

class BaggingModel : IModel {

private:
  // Parameters
  IOperator *split_Metric;
  ICriteria *split_Criteria;
  RandomForest forest;
  int max_Depth;

public:
  // Constructor

  BaggingModel() = delete;
  BaggingModel(const std::string &split_Metric,
               const std::string &split_Criteria, int max_Depth);

  // Getter

  int get_Depth();
  int get_Trees_Number();
  const std::map<int, DecisionTree> &get_Forest();

  // Setters

  void set_Metric(const std::string &metric);
  void set_Criteria(const std::string &criteria);

  // Destructor

  ~BaggingModel() override;

  // Methods
  void aggregate_Forest(const std::map<int, DecisionTree> &forest);
  void train(const DataSet &data) override;
  void train(const DataSet &data, int n);
  std::vector<double> predict(const DataSet &datas);
};

#endif