#ifndef CROSS_VALIDATION_H_
#define CROSS_VALIDATION_H_

#include "BaggingModel.hpp"
#include "DataSet.hpp"
#include "Validation.hpp"

namespace CrossValidation {

/*
Computes the train dataset index on a test index set
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
Parameters : Dataset, Global index, Total size of DataSet, K (nb of folds)
Inputs     : const DataSet, const vector<int>, int, int
Outputs    : tuple<vector<DataSet>, vector<DataSet>>
*/
std::tuple<std::vector<DataSet>, std::vector<DataSet>>
compute_Folds(const DataSet data, const std::vector<int> &global_Index,
              int total_Size, int K) {
  int foldSize = total_Size / K;

  std::vector<DataSet> test_Folds(K);
  std::vector<DataSet> train_Folds(K);

  for (int j = 0; j < K; ++j) {

    std::vector<int> test_Folds_Construct(foldSize, 0);
    int test_Curr_Index = 0;

    std::vector<int> train_Folds_Construct(total_Size - foldSize + 1, 0);
    int train_Curr_Index = 0;

    int upper_Bound = (j + 1) * foldSize;
    int lower_Bound = j * foldSize;
    for (int i = 0; i < total_Size; ++i) {
      if ((i > lower_Bound) && (i < upper_Bound)) {
        test_Folds_Construct[test_Curr_Index] = global_Index[i];
        test_Curr_Index++;
      } else {
        train_Folds_Construct[train_Curr_Index] = global_Index[i];
        train_Curr_Index++;
      }
    }
    test_Folds[j] = DataSet(data, test_Folds_Construct);
    train_Folds[j] = DataSet(data, train_Folds_Construct);
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
    perror("K-Folds methods needs at least K=2");
    return {};
  }

  double global_MAE = 0;
  double global_MAPE = 0;
  double global_Std_Dev = 0;

  int total_Size = data.samples_Number();
  std::vector<int> global_Index(total_Size, 0);

  // Computes the index for the given DataSet
  for (int i = 0; i < total_Size; ++i) {
    global_Index[i] = i;
  }

  // Computes the indexes of the folds
  auto [test_Folds, train_Folds] =
      compute_Folds(data, global_Index, total_Size, K);

  for (int i = 0; i < K; ++i) {
    // Creating Test Dataset for this iteration
    DataSet test_Set = test_Folds[i];

    // Creating Training Dataset for this iteration
    DataSet train_Set = train_Folds[i];

    // Train the model on the sub data set
    model.train(train_Set);

    auto [fold_Mae, fold_Mape, fold_Std_Dev] =
        metric::compute_accuracy(model, test_Set);

    // Add the result
    global_MAE += fold_Mae;
    global_MAPE += fold_Mape;
    global_Std_Dev += fold_Std_Dev;

  }

  // Means the result
  global_MAE /= K;
  global_MAPE /= K;
  global_Std_Dev /= K;

  //Printing everything to the user
  std::cout << "\n=== RESULTS OF CROSS-VALIDATION ===\n";
  std::cout << "\nGlobal Mean Absolute Error            : " << global_MAE
            << " for " << K << " folds\n";
  std::cout << "Global Mean Absolute Percentage Error : " << global_MAPE
            << " for " << K << " folds\n";
  std::cout << "Global Standard Deviation             : " << global_Std_Dev
            << " for " << K << " folds\n";
  std::cout << std::endl;

  return std::make_tuple(global_MAE, global_MAPE);
}
} // namespace CrossValidation

#endif