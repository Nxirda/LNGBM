#ifndef ENUM_OPERATOR_H_
#define ENUM_OPERATOR_H_

#include <map>

#include "IOperator.hpp"

#include "MAE.hpp"
#include "MAPE.hpp"
#include "RMSE.hpp"
#include "ReductionInVar.hpp"

namespace operators {

/**/
enum class type { MAE, MAPE, RMSE, ReductionInVar, Other };

/**/
inline std::map<std::string, type> dictionnary = {
    {"MAE", type::MAE},
    {"MAPE", type::MAPE},
    {"RMSE", type::RMSE},
    {"RIV", type::ReductionInVar}};

/**/
inline void print() {
  for (auto const &pair : dictionnary) {
    std::cout << "{" << pair.first << "}\n";
  }
  return;
}
} // namespace operators

#endif