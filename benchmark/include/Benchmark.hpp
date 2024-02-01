#ifndef BENCHMARK_H_
#define BENCHMARK_H_

#include "BaggingModel.hpp"
#include "DataSet.hpp"

void dataSet_Loading(std::string PATH);

void RIV_Benchmark(std::string PATH, int tree_Depth, int nb_Trees, int K);

void MAE_Benchmark(std::string PATH, int tree_Depth, int nb_Trees, int K);

void MAPE_Benchmark(std::string PATH, int tree_Depth, int nb_Trees, int K);

void RMSE_Benchmark(std::string PATH, int tree_Depth, int nb_Trees, int K);

// CPU Cycles counter
/* uint64_t rdtsc() {
  unsigned int lo, hi;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return ((uint64_t)hi << 32) | lo;
}; */

#endif