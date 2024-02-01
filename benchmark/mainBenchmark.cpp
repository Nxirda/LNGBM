#include "Benchmark.hpp"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cout << "Usage is : " << argv[0]
              << " [DataSet Path] [Tree Depth] [Number of Trees] [Number of Folds] \n"
              << "Folds are for cross Validation\n";
    return 1;
  }

  RIV_Benchmark(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
  std::cout << "\n";

  MAE_Benchmark(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
  std::cout << "\n";

  MAPE_Benchmark(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
  std::cout << "\n";

  RMSE_Benchmark(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
  std::cout << std::endl;
  
  return 0;
}