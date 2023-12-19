#ifndef VALIDATION_H_
#define VALIDATION_H_

#include "BaggingModel.hpp"
#include "MAE.hpp"
#include "MAPE.hpp"
#include <numeric>

namespace metric {
/*
Computes the accuracy of the given model
Parameters : Prediction Model, DataSet
Inputs     : BaggingModel, const DataSet
Outputs    : double
*/
std::tuple<double, double> compute_accuracy(BaggingModel &model, const DataSet &data) {

  auto prediction = model.predict(data);
  auto exact = data.get_Labels();
  
  std::cout << "\n===== PREDICTED RESULTS ===== \n";

  auto error = MAE::apply(exact, prediction);
  auto percentage_Error = MAPE::apply(exact, prediction);

  std::cout << "===== ERROR =====\n";
  std::cout << "Mean Absolute Error            : " << error << "\n";
  std::cout << "Mean Absolute Percentage Error : " << percentage_Error << "%\n";
  return std::make_tuple(error, percentage_Error);
}

}; // namespace metric

#endif