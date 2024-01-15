#include "BaggingModel.hpp"
#include "CrossValidation.hpp"
#include "DataSet.hpp"
#include "Validation.hpp"

#include <stdio.h>

using namespace std;

/*********************/
/*                   */
/*    TEMPORARY      */
/*                   */
/*********************/

int main(int argc, char **argv) {

  if (argc != 5) {
    std::cout
        << "Usage is : " << argv[0]
        << " [Path to DataSet] [Split Metric] [Depth] [Number of Trees]\n";
    std::cout << "\n";
    std::cout << "== Split Metrics Available are ==\n";
    BaggingModel m{};
    m.print_Available_Operators();
    return 1;
  }

  std::string dataset_Path = argv[1];
  std::string metric = argv[2];
  int depth = std::stoi(argv[3]);
  int number_Of_Trees = std::atoi(argv[4]);

  BaggingModel model{metric, depth};

  DataSet DS{dataset_Path};

  model.train(DS, number_Of_Trees);

  CrossValidation::K_Folds(model, DS, 5);

  /*  DataSet test_DS{};
  test_DS.load("../data/datasets/d1_Test.csv");
  DataSet test_DS = DataSet::load("../data/d1_Test.csv");
  metric::compute_accuracy(model, test_DS);*/
  return 0;
}