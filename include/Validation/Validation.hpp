#ifndef VALIDATION_H_
#define VALIDATION_H_

#include "BaggingModel.hpp"
#include "MAE.hpp"
#include "MAPE.hpp"
#include <numeric>

namespace metric {
double compute_accuracy(BaggingModel &model, const DataSet &data) {

  auto prediction = model.predict(data);
  auto exact = data.get_Labels();
  
  std::cout << "\n===== PREDICTED RESULTS ===== \n";
  //std::vector<float> result = model.predict(test_DS);
  for (auto idx : prediction) {
    std::cout << "[" << idx << "]";
  }
  std::cout << "\n"; 

  auto error = MAE::apply(exact, prediction);
  auto percentage_Error = MAPE::apply(exact, prediction);

  std::cout << "===== ERROR =====\n";
  std::cout << "MAE  : " << error << "\n";
  std::cout << "MAPE : " << percentage_Error << "%\n";
  return error;
}

}; // namespace metric

#endif