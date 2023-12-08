#include "BaggingModel.hpp"
#include "DataSet.hpp"
#include "EnumOperator.hpp"

#include <stdio.h>

using namespace std;

uint64_t rdtsc() {
  unsigned int lo, hi;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return ((uint64_t)hi << 32) | lo;
}

void Benchmark_Execution_Time(std::string PATH) {

  double cpu_frequency = 1.4e9;

  cout << "=== DataSet Loading ===\n";
  uint64_t t_DataSet = rdtsc();
  DataSet DS{PATH};
  uint64_t t_DataSet_Final = rdtsc();
  std::cout << "Loading the dataset took                    : "
            << t_DataSet_Final - t_DataSet << " CPU cycles\n";
  std::cout << "Loading the dataset took                    : "
            << (t_DataSet_Final - t_DataSet) / cpu_frequency << " seconds\n";
  cout << "=======================\n";

  cout << "=== Creating Shared ptr of DataSet ===\n";
  uint64_t t_Shared_DS = rdtsc();
  std::shared_ptr<DataSet> DS2 = std::make_shared<DataSet>(DS);
  uint64_t t_Shared_DS_Final = rdtsc();
  std::cout << "Making shared ptr of dataset took          : "
            << t_Shared_DS_Final - t_Shared_DS << " CPU cycles\n";
  std::cout << "Making shared ptr of dataset took          : "
            << (t_Shared_DS_Final - t_Shared_DS) / cpu_frequency
            << " seconds\n";
  cout << "=======================================\n";

  cout << "=== Creating Tree Node ===\n";
  uint64_t t_Node = rdtsc();
  TreeNode TN{DS2};
  uint64_t t_Node_Final = rdtsc();
  std::cout << "Making a Node of the dataset took          : "
            << t_Node_Final - t_Node << " CPU cycles\n";
  std::cout << "Making a Node of the dataset took          : "
            << (t_Node_Final - t_Node) / cpu_frequency << " seconds\n";
  cout << "==========================\n";

  cout << "=== Creating One Node Tree ===\n";
  uint64_t t_Tree = rdtsc();
  DecisionTree DT{DS};
  uint64_t t_Tree_Final = rdtsc();
  std::cout << "Buidling one node Tree of the dataset took : "
            << t_Tree_Final - t_Tree << " CPU cycles\n";
  std::cout << "Buidling one node Tree of the dataset took : "
            << (t_Tree_Final - t_Tree) / cpu_frequency << " seconds\n";
  cout << "==============================\n";

  cout << "=== Build the tree via splitting ===\n";
  cout << "===== Reduction in Variance =====\n";
  // Setting up the operator to be RIV
  IOperator *RIV_op = new ReductionInVar();
  RIV_op->set_Node(DT.get_Current_Node());
  DT.add_Operator(RIV_op);

  uint64_t t_Split_RIV = rdtsc();
  DT.build_Splitted_Tree(5);
  uint64_t t_Split_RIV_Final = rdtsc();
  std::cout << "Splitting at depth 5 took                  : "
            << t_Split_RIV_Final - t_Split_RIV << " CPU cycles\n";
  std::cout << "Splitting at depth 5 took                  : "
            << (t_Split_RIV_Final - t_Split_RIV) / cpu_frequency
            << " seconds\n";
  cout << "===================================\n";

  cout << "===== Root Mean Square Error =====\n";
  // Setting up the operator to be RMSE
  IOperator *RMSE_op = new RMSE();
  RMSE_op->set_Node(DT.get_Current_Node());
  DT.add_Operator(RMSE_op);

  uint64_t t_Split_RMSE = rdtsc();
  DT.build_Splitted_Tree(5);
  uint64_t t_Split_RMSE_Final = rdtsc();
  std::cout << "Splitting at depth 5 took                  : "
            << t_Split_RMSE_Final - t_Split_RMSE << " CPU cycles\n";
  std::cout << "Splitting at depth 5 took                  : "
            << (t_Split_RMSE_Final - t_Split_RMSE) / cpu_frequency
            << " seconds\n";
  cout << "===================================\n";

  cout << "===== Mean Absolute Error =====\n";
  // Setting up the operator to be MAE
  IOperator *MAE_op = new MAE();
  MAE_op->set_Node(DT.get_Current_Node());
  DT.add_Operator(MAE_op);

  uint64_t t_Split_MAE = rdtsc();
  DT.build_Splitted_Tree(5);
  uint64_t t_Split_MAE_Final = rdtsc();
  std::cout << "Splitting at depth 5 took                  : "
            << t_Split_MAE_Final - t_Split_MAE << " CPU cycles\n";
  std::cout << "Splitting at depth 5 took                  : "
            << (t_Split_MAE_Final - t_Split_MAE) / cpu_frequency
            << " seconds\n";
  cout << "===================================\n";

  cout << "===== Mean Absolute Percentage Error =====\n";
  // Setting up the operator to be MAPE
  IOperator *MAPE_op = new MAPE();
  MAPE_op->set_Node(DT.get_Current_Node());
  DT.add_Operator(MAPE_op);

  uint64_t t_Split_MAPE = rdtsc();
  DT.build_Splitted_Tree(5);
  uint64_t t_Split_MAPE_Final = rdtsc();
  std::cout << "Splitting at depth 5 took                  : "
            << t_Split_MAPE_Final - t_Split_MAPE << " CPU cycles\n";
  std::cout << "Splitting at depth 5 took                  : "
            << (t_Split_MAPE_Final - t_Split_MAPE) / cpu_frequency
            << " seconds\n";
  cout << "===================================\n";

  /* std::string metric = argv[1];
  int depth = std::stoi(argv[2]); */

  // Splitting Test
  /*  uint64_t t2 = rdtsc();
   BaggingModel model{metric, depth};
   model.fit(DS);
   uint64_t t3 = rdtsc();
   std::cout << "Splitting at depth "<< depth <<" took                  : " <<
   t3 - t2
             << " CPU cycles\n";
   std::cout << "Splitting at depth "<< depth <<" took                  : "
             << (t3 - t2) / cpu_frequency << " seconds\n";

   DataSet Test{};
   Test.load("../data/datasets/d1.csv"); */

  // Prediction Test
  /* uint64_t t4 = rdtsc();
  model.predict(Test);
  uint64_t t5 = rdtsc();
  std::cout << "Predicting at depth "<< depth <<" took                 : " << t5
  - t4
            << " CPU cycles\n";
  std::cout << "Predicting at depth "<< depth <<" took                 : "
            << (t5 - t4) / cpu_frequency << " seconds\n";    */

  /* std::cout << "\n===== BAGGING =====\n";
  BaggingModel model2{metric, depth};
  DataSet DS2{"../data/datasets/d1_Test.csv"};
  DataSet test_DS2{};
  test_DS2.load("../data/datasets/d1_Test.csv");

  // Bagging Test
  uint64_t t6 = rdtsc();
  model2.train(DS2, 25);
  uint64_t t7 = rdtsc();
  std::cout << "Bagging at depth " << depth
            << " took                 : " << t7 - t6 << " CPU cycles\n";
  std::cout << "Bagging at depth " << depth
            << " took                 : " << (t7 - t6) / cpu_frequency
            << " seconds\n";

  uint64_t t8 = rdtsc();
  model2.predict(test_DS2);
  uint64_t t9 = rdtsc();
  std::cout << "Predicting at depth " << depth
            << " took                 : " << t9 - t8 << " CPU cycles\n";
  std::cout << "Predicting at depth " << depth
            << " took                 : " << (t9 - t8) / cpu_frequency
            << " seconds\n"; */
}