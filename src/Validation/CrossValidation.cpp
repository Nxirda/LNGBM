#include "CrossValidation.hpp"

#include "Serializer.hpp"
#include "Timer.hpp"
#include "Tools.hpp"

#include <iomanip>

namespace CrossValidation {

//
std::vector<size_t> compute_Train(const std::vector<size_t> &global_Index,
                                  const std::vector<size_t> &test_Index) {
  std::vector<size_t> res(global_Index.size() - test_Index.size());
  // To track where we are in res index
  size_t cpt = 0;

  for (size_t i = 0; i < global_Index.size(); ++i) {
    if (global_Index[i] == test_Index[0]) {
      i += (test_Index.size() - 1);
    }
    res[cpt] = global_Index[i];
    cpt++;
  }

  return res;
}

//
std::tuple<std::vector<DataSet>, std::vector<DataSet>>
compute_Folds(const DataSet &data, const std::vector<size_t> &global_Index,
              size_t total_Size, uint8_t K) {

  const size_t fold_Size = total_Size / K;

  std::vector<DataSet> test_Folds(K);
  std::vector<DataSet> train_Folds(K);

  for (uint8_t j = 0; j < K; ++j) {

    std::vector<size_t> test_Folds_Construct(fold_Size, 0);
    size_t test_Curr_Index = 0;

    std::vector<size_t> train_Folds_Construct(total_Size - fold_Size + 1, 0);
    size_t train_Curr_Index = 0;

    size_t upper_Bound = (j + 1) * fold_Size;
    size_t lower_Bound = j * fold_Size;

    for (size_t i = 0; i < total_Size; ++i) {
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
  return std::make_tuple(std::move(test_Folds), std::move(train_Folds));
}

//
Answers K_Folds(BaggingModel &model, const DataSet &data, uint8_t K) {
  if (K <= 1) {
    std::cerr << "K-Folds methods needs at least K=2\n";
    return {};
  }

  Answers validation_Result{};

  // Getting infos on the model
  const int precision = 5;

  //
  std::string formatted_File_Size =
      Tools::matrix_Memory_Size(data.samples_Number(), data.features_Number(),
                                data.element_Size(), precision);

  // Initialize header for the metrics we want
  validation_Result.set_Header({"Folds", "Depth", "Trees", "File_size",
                                "Train_time", "MAE", "MAPE", "Std_dev"});

  double global_MAE = 0.0;
  double global_MAPE = 0.0;
  double global_Std_Dev = 0.0;

  // Size is K + 1 : we need results from each folds and the global result
  std::vector<std::vector<std::string>> global_Result(
      K + 1, std::vector<std::string>());

  std::vector<std::vector<double>> global_Numerical_Result(
      K + 1, std::vector<double>());

  // Initialize timer so it count the whole time the function is "really"
  // computing stuff
  Timer t_Global;
  t_Global.start();

  // Actual infos for the function
  const size_t total_Size = data.samples_Number();
  std::vector<size_t> global_Index(total_Size, 0);

  // Computes the index for the given DataSet
  for (size_t i = 0; i < total_Size; ++i) {
    global_Index[i] = i;
  }

  // Computes the indexes of the folds
  auto [test_Folds, train_Folds] =
      compute_Folds(data, global_Index, total_Size, K);

  for (uint8_t i = 0; i < K; ++i) {

    std::vector<std::string> inner_Values;
    std::vector<double> inner_Numerical_Values;

    // Creating Test Dataset for this iteration
    DataSet test_Set = test_Folds[i];

    // Creating Training Dataset for this iteration
    DataSet train_Set = train_Folds[i];

    Timer t_Intern;
    t_Intern.start();

    // Train the model on the sub data set
    model.train(train_Set);

    t_Intern.stop();

    auto [fold_Mae, fold_Mape, fold_Std_Dev] =
        metric::compute_accuracy(model, test_Set);

    std::string inner_Formatted_File_Size = Tools::matrix_Memory_Size(
        train_Set.samples_Number(), data.features_Number(), data.element_Size(),
        precision);

    inner_Values = {"Folds n*" + std::to_string(i),
                    std::to_string(model.get_Depth()),
                    std::to_string(model.get_Trees_Number()),
                    inner_Formatted_File_Size, t_Intern.get_Duration()};

    inner_Numerical_Values = {fold_Mae, fold_Mape, fold_Std_Dev};

    // Add the result
    global_MAE += fold_Mae;
    global_MAPE += fold_Mape;
    global_Std_Dev += fold_Std_Dev;

    // Update the vectors to build the answer class
    global_Result[i] = inner_Values;
    global_Numerical_Result[i] = inner_Numerical_Values;
  }

  // Means the result
  global_MAE /= K;
  global_MAPE /= K;
  global_Std_Dev /= K;

  t_Global.stop();

  // Stores the final string values after the folds
  std::vector<std::string> global_Values = {
      "Global " + std::to_string(K) + " Folds",
      std::to_string(model.get_Depth()),
      std::to_string(model.get_Trees_Number()), formatted_File_Size,
      t_Global.get_Duration()};

  // Stores the final numerical result after the folds
  std::vector<double> global_Numerical_Values = {global_MAE, global_MAPE,
                                                 global_Std_Dev};

  // In the end it contains each fold values and the whole result
  global_Result[K] = global_Values;
  global_Numerical_Result[K] = global_Numerical_Values;

  validation_Result.set_Values(global_Result);
  validation_Result.set_Numeric_Values(global_Numerical_Result);

  return validation_Result;
}
} // namespace CrossValidation