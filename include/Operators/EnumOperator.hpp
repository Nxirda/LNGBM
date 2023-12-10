#ifndef ENUM_OPERATOR_H_
#define ENUM_OPERATOR_H_

#include <map>

#include "IOperator.hpp"

#include "MAE.hpp"
//#include "MAPE.hpp"
//#include "RMSE.hpp"
//#include "ReductionInVar.hpp"

/**/
enum class OperatorType {
  MAE,
  MAPE,
  RMSE,
  ReductionInVar,
  Other

};

/**/
std::map<std::string, OperatorType> operator_Dictionnary = {
    {"MAE", OperatorType::MAE},
    {"MAPE", OperatorType::MAPE},
    {"RMSE", OperatorType::RMSE},
    {"RIV", OperatorType::ReductionInVar}};

#endif