#include "BaggingModel.hpp"
#include "DataSet.hpp"

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

  // For testing on a cutted dataset
  DataSet DS2{"../data/datasets/d1_Test.csv"};

  DataSet test_DS2{};
  test_DS2.load("../data/datasets/d1_Test.csv");

  model.train(DS2, 1);

  std::vector<float> result = model.predict(test_DS2);
  std::cout << "\n ===== MAIN RESULTS ===== \n";
  for (auto idx : result) {
    std::cout << "[" << idx << "]";
  }
  std::cout << "\n"; 

  /* DataSet DS{"../data/datasets/d1.csv"};

  DataSet test_DS{};
  test_DS.load("../data/datasets/d1_Test.csv");

  model.train(DS, 75);

  std::vector<float> result = model.predict(test_DS);
  for (auto idx : result) {
    std::cout << "[" << idx << "]";
  }
  std::cout << "\n"; */

  return 0;
}