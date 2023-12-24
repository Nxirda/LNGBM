#ifndef CROSS_VALIDATION_H_
#define CROSS_VALIDATION_H_

#include "BaggingModel.hpp"
#include "DataSet.hpp"
#include "Validation.hpp"

namespace CrossValidation {

/*
Computes the train dataset index after a test index set
Parameters : Global index, Test index
Inputs     : vector<int>, vector<int>
Outputs    : vector<int>
*/
std::vector<int> compute_Train(std::vector<int> global_Index,
                               std::vector<int> test_Index) {
  std::vector<int> res(global_Index.size() - test_Index.size());
  // To track where we are in res index
  int cpt = 0;

  for (unsigned long int i = 0; i < global_Index.size(); ++i) {
    if (global_Index[i] == test_Index[0]) {
      i += (test_Index.size() - 1);
    }
    res[cpt] = global_Index[i];
    cpt++;
  }

  return res;
}

/*
Computes indexes for the folds on the DataSet
Parameters : Global index, Total size of DataSet, K (nb of folds)
Inputs     : const vector<int>, int, int
Outputs    : tuple<vector<vector<int>>, vector<vector<int>>>
*/
std::tuple<std::vector<std::vector<int>>, std::vector<std::vector<int>>>
compute_Folds(const std::vector<int> &global_Index, int total_Size, int K) {
  int foldSize = total_Size / K;
  // int extra = total_Size % K;
  // int dataIndex = 0;

  std::vector<std::vector<int>> test_Folds(K);
  std::vector<std::vector<int>> train_Folds(K);

  for (int j = 0; j < K; ++j) {
    std::vector<int> test_Folds_Construct(foldSize);
    int test_Curr_Index = 0;

    std::vector<int> train_Folds_Construct(total_Size - foldSize);
    int train_Curr_Index = 0;

    int upper_Bound = (j + 1) * foldSize;
    int lower_Bound = j * foldSize;

    for (int i = 0; i < total_Size; ++i) {
      if ((i > lower_Bound) && (i < upper_Bound)) {
        test_Folds_Construct[test_Curr_Index] = global_Index[i];
      } else {
        train_Folds_Construct[train_Curr_Index] = global_Index[i];
      }
    }
    test_Folds[j] = test_Folds_Construct;
    train_Folds[j] = train_Folds_Construct;
  }
  return std::make_tuple(test_Folds, train_Folds);
}

/*
Computes the accuracy of the given model via K-Fold
Parameters : Prediction Model, DataSet, K
Inputs     : BaggingModel, const DataSet, int
Outputs    : double
*/
std::tuple<double, double> K_Folds(BaggingModel &model, const DataSet &data,
                                   int K) {
  if (K <= 1) {
    return {};
  }
  double global_MAE = 0;
  double global_MAPE = 0;

  int total_Size = data.samples_Number();
  std::vector<int> global_Index(total_Size);

  // Computes the index for the given DataSet
  for (int i = 0; i < data.samples_Number(); ++i) {
    global_Index[i] = i;
  }

  // Computes the indexes of the folds
  auto [test_Folds, train_Folds] = compute_Folds(global_Index, total_Size, K);

  for (int i = 0; i < K; ++i) {

    // Creating Test Dataset for this iteration
    DataSet test(data, test_Folds[i]);

    std::cout << "Training for Fold ["<< i << "]\n";
    std::cout << test_Folds[i].size() << "\n";
    std::cout << train_Folds[i].size() << "\n";
    // Creating Training Dataset for this iteration
    DataSet train(data, train_Folds[i]);

    // Train the model on the sub data set
    model.train(train);

    auto [fold_Mae, fold_Mape] = metric::compute_accuracy(model, test);
    // Add the result
    global_MAE += fold_Mae;
    global_MAPE += fold_Mape;
  }

  // Means the result
  global_MAE /= K;
  global_MAPE /= K;

  std::cout << "\n=== RESULTS OF CROSS-VALIDATION ===\n";
  std::cout << "\nGlobal Mean Absolute Error            : " << global_MAE
            << " for " << K << " folds\n";
  std::cout << "Global Mean Absolute Percentage Error : " << global_MAPE
            << " for " << K << " folds\n";
  std::cout << std::endl;

  return std::make_tuple(global_MAE, global_MAPE);
}
} // namespace CrossValidation

#endif