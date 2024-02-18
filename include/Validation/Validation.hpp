#ifndef VALIDATION_H_
#define VALIDATION_H_

#include "BaggingModel.hpp"

namespace metric {
/*
Computes the accuracy of the given model
Parameters : Prediction Model, DataSet
Inputs     : BaggingModel, const DataSet
Outputs    : double
*/
std::tuple<double, double, double> compute_accuracy(BaggingModel &model,
                                                    const DataSet &data);
}; // namespace metric

#endif