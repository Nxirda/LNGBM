#include <stdio.h>
#include "DataSet.hpp"
#include "DecisionTree.hpp"

using namespace std;

/*********************/
/*                   */
/*    TEMPORARY      */
/*                   */
/*********************/

uint64_t rdtsc() {
  unsigned int lo, hi;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return ((uint64_t)hi << 32) | lo;
}

int main() {

  cout << "=== DataSet Loading ===\n";

  uint64_t t0 = rdtsc();
  DataSet DS{"../data/datasets/d1.csv"};
  uint64_t t1 = rdtsc();
  std::cout << "Loading the dataset took : " << t1 - t0 << " CPU cycles\n";

  uint64_t t_Shared_DS = rdtsc();
  std::shared_ptr<DataSet> DS2 = std::make_shared<DataSet>(DS);
  uint64_t t_Shared_DS_F = rdtsc();
  std::cout << "Making shared ptr of dataset took : " << t_Shared_DS_F - t_Shared_DS << " CPU cycles\n";

  uint64_t t4 = rdtsc();
  TreeNode TN{DS2};
  uint64_t t5 = rdtsc();
  std::cout << "Making a Node of the dataset took : " << t5 - t4 << " CPU cycles\n";

  //uint64_t t2 = rdtsc();
  //DecisionTree DT{DS};
  //uint64_t t3 = rdtsc();
  //std::cout << "Buidling one node Tree of the dataset took : " << t3 - t2 << " CPU cycles\n";

  //uint64_t t4 = rdtsc();
  //DT.build_Splitted_Tree(1);
  //uint64_t t5 = rdtsc();
  //std::cout << "Splitting at depth 5 took : " << t5 - t4 << " CPU cycles\n";

  return 0;
}