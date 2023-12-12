#include "BaggingModel.hpp"
#include "DataSet.hpp"
//#include "RandomForest.hpp"
//#include "EnumOperator.hpp"

#include <stdio.h>

using namespace std;

/*********************/
/*                   */
/*    TEMPORARY      */
/*                   */
/*********************/

/* uint64_t rdtsc() {
  unsigned int lo, hi;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return ((uint64_t)hi << 32) | lo;
} */

int main(int argc, char **argv) {

  if (argc < 3) {
    std::cout << "Usage is : " << argv[0] << " [Split Metric] [Depth]\n";
    std::cout << "\n";
    std::cout << "== Split Metrics Available are ==\n";
    /*  BaggingModel m{};
     m.print_Available_Operators(); */
    return 1;
  }

  std::string metric = argv[1];
  int depth = std::stoi(argv[2]);
  /* DataSet DS{"../data/datasets/d1_Test.csv"};

  IOperator* split_Man = new MAE();
  RandomForest rf(DS, split_Man, 1, depth);
  rf.generate_Forest(25);

  DataSet test_DS{};
  test_DS.load("../data/datasets/d1_Test.csv");

  std::vector<float> res = rf.predict_Results(test_DS);
  for(unsigned long int i = 0; i < res.size(); ++i){
    std::cout << "[" << res[i] << "]";
  }
  std::cout << endl; */

  BaggingModel model{metric, depth};

  // For testing on a cutted dataset
  DataSet DS2{"../data/datasets/d1_Test.csv"};

  DataSet test_DS2{};
  test_DS2.load("../data/datasets/d1_Test.csv");

  model.train(DS2, 25);

  std::vector<float> result = model.predict(test_DS2);
  for (auto idx : result) {
    std::cout << "[" << idx << "]";
  }
  std::cout << "\n";

  return 0;
}