#ifndef ENUM_OPERATOR_H_
#define ENUM_OPERATOR_H_

#include <map>

#include "IOperator.hpp"

#include "MAE.hpp"
#include "MAPE.hpp"
#include "RIV.hpp"
#include "RMSE.hpp"

namespace operators {

/**/
enum class type { MAE, MAPE, RMSE, RIV, Other };

/**/
extern std::map<std::string, type> dictionnary;

/**/
void print();
} // namespace operators

#endif