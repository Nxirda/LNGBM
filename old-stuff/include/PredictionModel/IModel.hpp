#ifndef I_MODEL_H_
#define I_MODEL_H_

#include "ICriteria.hpp"
#include "IOperator.hpp"
#include <DataSet.hpp>

class IModel {
public:
  virtual ~IModel(){};
  /*   virtual void train(const DataSet &data) = 0;
   */
  virtual void train(const DataSet &data, uint64_t bins) = 0;
  virtual void train(const DataSet &data, ICriteria *crit, IOperator *op) = 0;
  virtual std::vector<double> predict(const DataSet &data) const = 0;
};

#endif