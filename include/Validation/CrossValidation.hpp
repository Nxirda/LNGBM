#ifndef CROSS_VALIDATION_H_
#define CROSS_VALIDATION_H_

#include "Answers.hpp"
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
std::vector<int> compute_Train(const std::vector<int> &global_Index,
                               const std::vector<int> &test_Index);

/*
Computes indexes for the folds on the DataSet
Parameters : Dataset, Global index, Total size of DataSet, K (nb of folds)
Inputs     : const DataSet, const vector<int>, int, int
Outputs    : tuple<vector<DataSet>, vector<DataSet>>
*/
std::tuple<std::vector<DataSet>, std::vector<DataSet>>
compute_Folds(const DataSet &data, const std::vector<int> &global_Index,
              int total_Size, int K);

/*
Computes the accuracy of the given model via K-Fold
Parameters : Prediction Model, DataSet, K
Inputs     : BaggingModel, const DataSet, int
Outputs    : double
*/
Answers K_Folds(BaggingModel &model, const DataSet &data, int K);
} // namespace CrossValidation

#endif