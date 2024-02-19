#ifndef I_MODEL_H_
#define I_MODEL_H_

#include <DataSet.hpp>
#include "IOperator.hpp"
#include "ICriteria.hpp"

class IModel {
    private:

    ICriteria *splitting_Criteria;
    IOperator *splitting_Operator;

    public:

    virtual ~IModel(){};
    virtual void train(const DataSet &data) = 0;
    virtual std::vector<double> predict(const DataSet &data) const = 0;
};

#endif