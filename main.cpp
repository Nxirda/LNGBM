#include "BaggingModel.hpp"
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

  DataSet test_DS{};
  test_DS.load("../data/datasets/d1_Test.csv");

  model.train(DS, 25);

  /* auto result = model.predict(test_DS);

  std::cout << "\n ===== MAIN RESULTS ===== \n";
  //std::vector<float> result = model.predict(test_DS);
  for (auto idx : result) {
    std::cout << "[" << idx << "]";
  }
  std::cout << "\n";  */

  metric::compute_accuracy(model, test_DS);
 /*  std::cout << "\n ===== ERROR ===== \n";
  std::cout << " Mean Asbolute Error : " << error << "\n"; */

  return 0;
}