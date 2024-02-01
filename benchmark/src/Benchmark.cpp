#include <fstream>
#include <stdio.h>

#include "Benchmark.hpp"
#include "CrossValidation.hpp"
#include "Timer.hpp"

//
void dataSet_Loading(std::string PATH) {
  Timer t;
  t.start();

  DataSet DS{PATH};

  t.stop();
  t.print("DataSet Loading took ");
}

//
void RIV_Benchmark(std::string PATH, int tree_Depth, int nb_Trees, int K) {
  std::cout << "      ===== RIV =====\n";
  DataSet ds{PATH};

  BaggingModel RIV_Bench("RIV","Q", tree_Depth);

  Timer t;
  t.start();
  RIV_Bench.train(ds, nb_Trees);
  t.stop();
  t.print("Reduction in variance splitting took ");

  t.start();
  CrossValidation::K_Folds(RIV_Bench, ds, K);
  t.stop();
  t.print("Reduction in variance cross-validation took ");
}

//
void MAE_Benchmark(std::string PATH, int tree_Depth, int nb_Trees, int K) {
  std::cout << "      ===== MAE =====\n";
  DataSet ds{PATH};

  BaggingModel MAE_Bench("MAE","Q", tree_Depth);

  Timer t;
  t.start();
  MAE_Bench.train(ds, nb_Trees);
  t.stop();
  t.print("Mean absolute error splitting took ");

  t.start();
  CrossValidation::K_Folds(MAE_Bench, ds, K);
  t.stop();
  t.print("Mean absolute error cross-validation took ");
}

//
void MAPE_Benchmark(std::string PATH, int tree_Depth, int nb_Trees, int K) {
  std::cout << "      ===== MAPE =====\n";
  DataSet ds{PATH};

  BaggingModel MAPE_Bench("RMSE","Q", tree_Depth);

  Timer t;
  t.start();
  MAPE_Bench.train(ds, nb_Trees);
  t.stop();
  t.print("Mean absolute percentage error splitting took ");

  t.start();
  CrossValidation::K_Folds(MAPE_Bench, ds, K);
  t.stop();
  t.print("Mean absolute percentage error cross-validation took ");
}

//
void RMSE_Benchmark(std::string PATH, int tree_Depth, int nb_Trees, int K) {
  std::cout << "      ===== RMSE =====\n";
  DataSet ds{PATH};
  BaggingModel RMSE_Bench("RMSE","Q", tree_Depth);

  Timer t;
  t.start();
  RMSE_Bench.train(ds, nb_Trees);
  t.stop();
  t.print("Mean absolute error splitting took ");

  t.start();
  CrossValidation::K_Folds(RMSE_Bench, ds, K);
  t.stop();
  t.print("Root mean square error cross-validation took ");
}