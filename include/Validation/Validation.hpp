#ifndef VALIDATION_H_
#define VALIDATION_H_

#include "BaggingModel.hpp"

namespace metric {
//
std::tuple<double, double, double> compute_accuracy(BaggingModel &model,
                                                    const DataSet &data);
}; // namespace metric

#endif