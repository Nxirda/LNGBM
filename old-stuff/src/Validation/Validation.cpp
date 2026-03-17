#include "Validation.hpp"

#include "MAE.hpp"
#include "MAPE.hpp"
#include "RIV.hpp"

#include <cmath>
#include <numeric>

namespace metric {
//
std::tuple<double, double, double> compute_accuracy(BaggingModel &model,
                                                    const DataSet &data) {
  std::vector<double> exact = data.get_Labels();
  std::vector<double> prediction = model.predict(data);

  double error = MAE::apply(exact, prediction);
  double percentage_Error = MAPE::apply(exact, prediction);
  double standard_deviation = std::sqrt(RIV::apply(exact, prediction));

  return std::make_tuple(error, percentage_Error, standard_deviation);
}

}; // namespace metric