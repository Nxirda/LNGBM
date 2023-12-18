#include "BaggingModel.hpp"
#include "DataSet.hpp"
#include "Validation.hpp"
#include "CrossValidation.hpp"

#include <stdio.h>

using namespace std;

/*********************/
/*                   */
/*    TEMPORARY      */
/*                   */
/*********************/

int main(int argc, char **argv) {

  if (argc < 3) {
    std::cout << "Usage is : " << argv[0] << " [Split Metric] [Depth]\n";
    std::cout << "\n";
    std::cout << "== Split Metrics Available are ==\n";
    BaggingModel m{};
    m.print_Available_Operators(); 
    return 1;
  }

  std::string metric = argv[1];
  int depth = std::stoi(argv[2]);

  BaggingModel model{metric, depth};

  DataSet DS{"../data/datasets/d1.csv"};

  //DataSet test_DS{};
  //test_DS.load("../data/datasets/d1_Test.csv");
  DataSet test_DS = DataSet::load("../data/datasets/d1_Test.csv");
  
  model.train(DS, 25);
  
  //metric::compute_accuracy(model, test_DS);

  CrossValidation::K_Folds(model, DS, 5);
  return 0;
}