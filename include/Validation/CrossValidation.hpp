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
Computes indexes for the DataSet folds for the validation metric
Parameters : Global index, Total size of DataSet, K (nb of folds)
Inputs     : const vector<int>, int, int
Outputs    : vector<vector<int>>
*/
std::vector<std::vector<int>>
compute_Folds(const std::vector<int> &global_Index, int total_Size, int K) {

  std::vector<std::vector<int>> folds(K);

  int foldSize = total_Size / K;
  int extra = total_Size % K;
  int dataIndex = 0;

  for (int i = 0; i < K; ++i) {
    int foldCount = (i < extra) ? foldSize + 1 : foldSize;
    std::vector<int> fold;

    for (int j = 0; j < foldCount; ++j) {
      fold.push_back(global_Index[dataIndex++]);
    }

    folds[i] = fold;
  }
  return folds;
}

/*
Computes the accuracy of the given model via K-Fold
Parameters : Prediction Model, DataSet, K
Inputs     : BaggingModel, const DataSet, int
Outputs    : double
*/
std::tuple<double, double> K_Folds(BaggingModel &model, const DataSet &data, int K) {
  std::cout << "===== METRIC : " << model.get_Metric() << " =====\n";
  if (K == 0) {
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

  // Computes the index of the folds
  std::vector<std::vector<int>> folds =
      compute_Folds(global_Index, total_Size, K);

  for (int i = 0; i < K; ++i) {

    // Creating Test Dataset for this iteration
    DataSet test(data, folds[i]);

    // Creating Training Dataset for this iteration
    std::vector<int> train_Index = compute_Train(global_Index, folds[i]);
    DataSet train(data, train_Index);

    // Train the model on the sub data set
    model.train(train);

    auto [fold_Mae, fold_Mape] = metric::compute_accuracy(model, test);
    // Add the result
    // global_Result += metric::compute_accuracy(model, test);
    global_MAE += fold_Mae;
    global_MAPE += fold_Mape;
  }

  // Means the result
  global_MAE /= K;
  global_MAPE /= K;

  std::cout << "\n===== RESULT OF CROSS-VALIDATION =====\n";
  std::cout << "Global Mean Absolute Error            : " << global_MAE << "\n";
  std::cout << "Global Mean Absolute Percentage Error : " << global_MAPE << "\n";
  std::cout << std::endl;

  return std::make_tuple(global_MAE, global_MAPE);
}
} // namespace CrossValidation

#endif