#ifndef I_MODEL_H_
#define I_MODEL_H_

#include <DataSet.hpp>

class IModel {
    private:

    public:

    virtual ~IModel(){};
    virtual void train(const DataSet &data) = 0;
    //virtual void predict() const = 0;
};

#endif