#include "BaggingModel.hpp"
#include "DataSet.hpp"

#include <stdio.h>

using namespace std;

uint64_t rdtsc() {
  unsigned int lo, hi;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return ((uint64_t)hi << 32) | lo;
}

void Benchmark_Execution_Time(std::string PATH) {
  DataSet test_DS{};
  test_DS.load(PATH);

  double cpu_frequency = 1.4e9;

  cout << "=== DataSet Loading ===\n";
  uint64_t t_DataSet = rdtsc();
  DataSet DS{PATH};
  uint64_t t_DataSet_Final = rdtsc();
  std::cout << "Loading the dataset took : " << t_DataSet_Final - t_DataSet
            << " CPU cycles\n";
  std::cout << "Loading the dataset took : "
            << (t_DataSet_Final - t_DataSet) / cpu_frequency << " seconds\n";
  cout << "===================================\n";

  cout << "===  Trees ===\n";
  cout << "\n";
  cout << "===== Reduction in Variance depth 10 =====\n";
  // Setting up the operator to be RIV
  BaggingModel RIV_Bench("RIV", 10);

  uint64_t t_Split_RIV = rdtsc();
  RIV_Bench.train(PATH, 1);
  uint64_t t_Split_RIV_Final = rdtsc();
  std::cout << "Splitting took  : " << t_Split_RIV_Final - t_Split_RIV
            << " CPU cycles\n";
  std::cout << "Splitting took  : "
            << (t_Split_RIV_Final - t_Split_RIV) / cpu_frequency
            << " seconds\n";

  cout << "\n";

  uint64_t t_Pred_RIV = rdtsc();
  RIV_Bench.predict(test_DS);
  uint64_t t_Pred_RIV_Final = rdtsc();
  std::cout << "Predicting took : " << t_Pred_RIV_Final - t_Pred_RIV
            << " CPU cycles\n";
  std::cout << "Predicting took : "
            << (t_Pred_RIV_Final - t_Pred_RIV) / cpu_frequency << " seconds\n";
  cout << "===================================\n";

  cout << "===== Root Mean Square Error depth 10 =====\n";
  // Setting up the operator to be RMSE
  BaggingModel RMSE_Bench("RMSE", 10);

  uint64_t t_Split_RMSE = rdtsc();
  RMSE_Bench.train(PATH, 1);
  uint64_t t_Split_RMSE_Final = rdtsc();
  std::cout << "Splitting took  : " << t_Split_RMSE_Final - t_Split_RMSE
            << " CPU cycles\n";
  std::cout << "Splitting took  : "
            << (t_Split_RMSE_Final - t_Split_RMSE) / cpu_frequency
            << " seconds\n";

  cout << "\n";

  uint64_t t_Pred_RMSE = rdtsc();
  RMSE_Bench.predict(test_DS);
  uint64_t t_Pred_RMSE_Final = rdtsc();
  std::cout << "Predicting took : " << t_Pred_RMSE_Final - t_Pred_RMSE
            << " CPU cycles\n";
  std::cout << "Predicting took : "
            << (t_Pred_RMSE_Final - t_Pred_RMSE) / cpu_frequency
            << " seconds\n";
  cout << "===================================\n";

  cout << "===== Mean Absolute Error depth 10 =====\n";
  // Setting up the operator to be MAE
  BaggingModel MAE_Bench("MAE", 1);

  uint64_t t_Split_MAE = rdtsc();
  MAE_Bench.train(PATH, 10);
  uint64_t t_Split_MAE_Final = rdtsc();
  std::cout << "Splitting took  : " << t_Split_MAE_Final - t_Split_MAE
            << " CPU cycles\n";
  std::cout << "Splitting took  : "
            << (t_Split_MAE_Final - t_Split_MAE) / cpu_frequency
            << " seconds\n";

  cout << "\n";

  uint64_t t_Pred_MAE = rdtsc();
  MAE_Bench.predict(test_DS);
  uint64_t t_Pred_MAE_Final = rdtsc();
  std::cout << "Predicting took : " << t_Pred_MAE_Final - t_Pred_MAE
            << " CPU cycles\n";
  std::cout << "Predicting took : "
            << (t_Pred_MAE_Final - t_Pred_MAE) / cpu_frequency << " seconds\n";
  cout << "===================================\n";

  cout << "===== Mean Absolute Percentage Error depth 10 =====\n";
  // Setting up the operator to be MAPE
  BaggingModel MAPE_Bench("RMSE", 1);

  uint64_t t_Split_MAPE = rdtsc();
  MAPE_Bench.train(PATH, 10);
  uint64_t t_Split_MAPE_Final = rdtsc();
  std::cout << "Splitting took  : " << t_Split_MAPE_Final - t_Split_MAPE
            << " CPU cycles\n";
  std::cout << "Splitting took  : "
            << (t_Split_MAPE_Final - t_Split_MAPE) / cpu_frequency
            << " seconds\n";
  cout << "\n";

  uint64_t t_Pred_MAPE = rdtsc();
  MAPE_Bench.predict(test_DS);
  uint64_t t_Pred_MAPE_Final = rdtsc();
  std::cout << "Predicting took : " << t_Pred_MAPE_Final - t_Pred_MAPE
            << " CPU cycles\n";
  std::cout << "Predicting took : "
            << (t_Pred_MAPE_Final - t_Pred_MAPE) / cpu_frequency
            << " seconds\n";
  cout << "===================================\n";
}

int main() {
  Benchmark_Execution_Time("../data/datasets/d1_Test.csv");
  return 0;
}