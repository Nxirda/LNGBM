#ifndef VALIDATION_H_
#define VALIDATION_H_

#include <numeric>
#include <cmath>

#include "MAE.hpp"
#include "MAPE.hpp"
#include "BaggingModel.hpp"
#include "RIV.hpp"

namespace metric {
/*
Computes the accuracy of the given model
Parameters : Prediction Model, DataSet
Inputs     : BaggingModel, const DataSet
Outputs    : double
*/
std::tuple<float, float, float> compute_accuracy(BaggingModel &model,
                                            const DataSet &data) {                                              
  std::vector<float> exact = data.get_Labels(); 
  std::vector<float> prediction = model.predict(data);

  float error = MAE::apply(exact, prediction);
  float percentage_Error = MAPE::apply(exact, prediction);
  float standard_deviation = std::sqrt(RIV::apply(exact, prediction));

  return std::make_tuple(error, percentage_Error, standard_deviation);
}

}; // namespace metric

#endif