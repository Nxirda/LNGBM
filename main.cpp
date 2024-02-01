#include "BaggingModel.hpp"
#include "CrossValidation.hpp"
#include "DataSet.hpp"

#include "EnumCriteria.hpp"
#include "EnumOperator.hpp"

#include "Validation.hpp"

#include <stdio.h>

using namespace std;

/*********************/
/*                   */
/*    TEMPORARY      */
/*                   */
/*********************/

int main(int argc, char **argv) {

  if (argc != 6) {
    std::cout << "Usage is : " << argv[0]
              << " [Path to DataSet] [Split Metric] [Split Criteria] [Depth] "
                 "[Number of Trees]\n";

    std::cout << "\n";

    std::cout << "== Split Metrics Available are ==\n";
    operators::print();

    std::cout << "== Split Criterias Available are ==\n";
    criterias::print();

    return 1;
  }

  std::string dataset_Path = argv[1];
  std::string metric = argv[2];
  std::string criteria = argv[3];
  int depth = std::stoi(argv[4]);
  int number_Of_Trees = std::atoi(argv[5]);

  BaggingModel model{metric, criteria, depth};

  DataSet DS{dataset_Path};

  model.train(DS, number_Of_Trees);

  CrossValidation::K_Folds(model, DS, 5);

  return 0;
}