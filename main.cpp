#include "BaggingModel.hpp"
#include "DataSet.hpp"

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
    BaggingModel m{};
    m.print_Available_Operators();
    return 1;
  }


  //DataSet DS{"../data/datasets/d1.csv"};

  std::string metric = argv[1];
  int depth = std::stoi(argv[2]);

  BaggingModel model2{metric, depth};

  //For testing on a cutted dataset
  DataSet DS2{"../data/datasets/d1_Test.csv"};
  
  DataSet test_DS2{};
  test_DS2.load("../data/datasets/d1_Test.csv");

  model2.train(DS2, 25);
  
  model2.predict(test_DS2);

  std::vector<float> result = model2.get_Prediction();
  for (auto idx : result) {
    std::cout << "[" << idx << "]";
  }
  std::cout << "\n";

  return 0;
}