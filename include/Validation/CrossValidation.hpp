#ifndef CROSS_VALIDATION_H_
#define CROSS_VALIDATION_H_

#include "BaggingModel.hpp"
#include "DataSet.hpp"
#include "Validation.hpp"

namespace CrossValidation {

/**/
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

/**/
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

/**/
double K_Folds(BaggingModel &model, const DataSet &data, int K) {
  std::cout << "===== METRIC : " << model.get_Metric() << " =====\n";
  if (K == 0) {
    return 0;
  }
  double global_Result = 0;
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

    //
    model.train(train);
    global_Result += metric::compute_accuracy(model, test);
  }
  global_Result /= K;

  std::cout << "===== RESULT OF CROSS-VALIDATION =====\n";
  std::cout << "Global MAE : " << global_Result << "\n";
  std::cout << std::endl;

  return global_Result;
}
} // namespace CrossValidation

#endif