#include "Benchmark.hpp"

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cout << "Usage is : " << argv[0]
              << " [DataSet Path] [Tree Depth] [Number of Trees]\n";
    return 1;
  }
  // Benchmark_Execution_Time("../data/datasets/d1_Test.csv");
  RIV_Benchmark(argv[1], atoi(argv[2]), atoi(argv[3]));
  std::cout << "\n";
  MAE_Benchmark(argv[1], atoi(argv[2]), atoi(argv[3]));
  std::cout << "\n";
  MAPE_Benchmark(argv[1], atoi(argv[2]), atoi(argv[3]));
  std::cout << "\n";
  RMSE_Benchmark(argv[1], atoi(argv[2]), atoi(argv[3]));
  std::cout << std::endl;
  return 0;
}