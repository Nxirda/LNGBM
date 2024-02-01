#ifndef ENUM_OPERATOR_H_
#define ENUM_OPERATOR_H_

#include <map>

#include "IOperator.hpp"

#include "MAE.hpp"
#include "MAPE.hpp"
#include "RMSE.hpp"
#include "RIV.hpp"

namespace operators {

/**/
enum class type { MAE, MAPE, RMSE, RIV, Other };

/**/
inline std::map<std::string, type> dictionnary = {
    {"MAE", type::MAE},
    {"MAPE", type::MAPE},
    {"RMSE", type::RMSE},
    {"RIV", type::RIV}};

/**/
inline void print() {
  for (auto const &pair : dictionnary) {
    std::cout << "{" << pair.first << "} : ";
    switch (pair.second) {
    case (operators::type::MAE):
      std::cout << MAE::get_Name();
      break;
    case (operators::type::MAPE):
      std::cout << MAPE::get_Name();
      break;
    case (operators::type::RMSE):
      std::cout << RMSE::get_Name();
      break;
    case (operators::type::RIV):
      std::cout << RIV::get_Name();
      break;
    default:
      std::cout << " Unknown Type";
    }

    std::cout << "\n";
  }
  return;
}
} // namespace operators

#endif