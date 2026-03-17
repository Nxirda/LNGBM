#include "EnumOperator.hpp"

namespace operators {
//
std::map<std::string, type> dictionnary = {{"MAE", type::MAE},
                                           {"MAPE", type::MAPE},
                                           {"RMSE", type::RMSE},
                                           {"RIV", type::RIV}};

//
void print() {
  for (auto const &pair : dictionnary) {
    std::cout << "{" << pair.first << "} : ";
    switch (pair.second) {
    case (operators::type::MAE):
      std::cout << MAE::get_Name_Static();
      break;
    case (operators::type::MAPE):
      std::cout << MAPE::get_Name_Static();
      break;
    case (operators::type::RMSE):
      std::cout << RMSE::get_Name_Static();
      break;
    case (operators::type::RIV):
      std::cout << RIV::get_Name_Static();
      break;
    default:
      std::cout << " Unknown Type";
    }

    std::cout << "\n";
  }
  return;
}
} // namespace operators