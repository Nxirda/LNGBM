#ifndef CROSS_VALIDATION_H_
#define CROSS_VALIDATION_H_

#include "Answers.hpp"
#include "BaggingModel.hpp"
#include "DataSet.hpp"

#include "Validation.hpp"

namespace CrossValidation {

//
std::vector<size_t> compute_Train(const std::vector<size_t> &global_Index,
                               const std::vector<size_t> &test_Index);

//
std::tuple<std::vector<DataSet>, std::vector<DataSet>>
compute_Folds(const DataSet &data, const std::vector<size_t> &global_Index,
              size_t total_Size, uint8_t K);

//
Answers K_Folds(BaggingModel &model, const DataSet &data, uint8_t K);
} // namespace CrossValidation

#endif