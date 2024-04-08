#ifndef BAGGING_MODEL_H_
#define BAGGING_MODEL_H_

#include "IModel.hpp"
#include "RandomForest.hpp"

class BaggingModel : IModel {

private:
  // Parameters
  RandomForest forest;
  uint16_t max_Depth;
  uint16_t number_Of_Trees;
  std::unique_ptr<ICriteria> split_Criteria;
  std::unique_ptr<IOperator> split_Operator;

public:
  // Constructor

  BaggingModel() noexcept; //= delete;
  BaggingModel(const std::string &split_Operator,
               const std::string &split_Criteria, uint16_t max_Depth,
               uint16_t number_Of_Trees) noexcept;

  BaggingModel(BaggingModel &&model) noexcept;
  BaggingModel &operator=(BaggingModel &&model) noexcept;

  // Getters

  ICriteria *get_Criteria() const;
  IOperator *get_Operator() const;

  uint16_t get_Depth() const;
  uint16_t get_Trees_Number() const;

  const std::unordered_map<uint16_t, DecisionTree> &get_Forest() const;

  // Setters
  void set_Depth(uint16_t depth);
  void set_Trees_Number(uint16_t number_Of_Trees);
  void set_Operator(const std::string &op);
  void set_Criteria(const std::string &criteria);

  // Destructor

  ~BaggingModel() override;

  // Methods

  void train(const DataSet &data);
  void train(const DataSet &data, ICriteria *crit, IOperator *op) override;
  std::vector<double> predict(const DataSet &datas) const override;
};

#endif