#ifndef CROSS_VALIDATION_H_
#define CROSS_VALIDATION_H_

#include <iomanip>
#include <mpi.h>

#include "BaggingModel.hpp"
#include "DataSet.hpp"
#include "Timer.hpp"
#include "Tools.hpp"
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
std::tuple<float, float> K_Folds(BaggingModel &model, const DataSet &data,
                                 int K) {
  if (K <= 1) {
    perror("K-Folds methods needs at least K=2");
    return {};
  }

  float global_MAE = 0;
  float global_MAPE = 0;
  float global_Std_Dev = 0;

  // Getting infos on the model
  int depth = model.get_Depth();
  int trees = model.get_Trees_Number(); 
  int width = data.features_Length();
  int element_Size = data.element_Size();
  int precision = 5;
  std::string formatted_File_Size = tools::matrix_Memory_Size(
      data.samples_Number(), width, element_Size, precision);

  // Initialize header for the metrics we want
  std::vector<std::string> header = {"Folds", "Depth", "Trees",   "File_size",
                                     "MAE",   "MAPE",  "Std_dev", "Train_time"};
  tools::display_Header(header); 

  // Initialize timer so it count the whole time the function takes
  Timer t_Global;
  t_Global.start();

  // Actual infos for the function
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

    std::string formatted_Matrix_Size = tools::matrix_Memory_Size(
        train_Set.samples_Number(), width, element_Size, precision);

    Timer t_Intern;
    t_Intern.start();

    // Train the model on the sub data set
    model.train(train_Set);

    t_Intern.stop();

    auto [fold_Mae, fold_Mape, fold_Std_Dev] =
        metric::compute_accuracy(model, test_Set);

    // Add the result
    global_MAE += fold_Mae;
    global_MAPE += fold_Mape;
    global_Std_Dev += fold_Std_Dev;

    //Send && receive values from MPI Process to print in proc 0
    //Only print once it has ended ppbly 
    //Serialize the values to display 

    tools::display_Values("Fold n*" + std::to_string(i), depth, trees,
                          formatted_Matrix_Size, fold_Mae, fold_Mape,
                          fold_Std_Dev, t_Intern.get_Duration());
  }

  // Means the result
  global_MAE /= K;
  global_MAPE /= K;
  global_Std_Dev /= K;

  t_Global.stop();

  tools::display_Values("Global", depth, trees, formatted_File_Size, global_MAE,
                        global_MAPE, global_Std_Dev, t_Global.get_Duration()); 

  return std::make_tuple(global_MAE, global_MAPE);
  
}
} // namespace CrossValidation

#endif

// Get the result of the other processes to aggregate them
  /* if (rank == 0) {
    int counter = 0;

    while (counter < size - 1) {
      int flag = 0;
      // Checks for incoming message
      MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);

      if (flag) {
        int filename_Size;
        MPI_Status status;

        // Get infos
        MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_CHAR, &filename_Size);

        std::vector<char> filename_Buffer(filename_Size);

        // Get message and handle infos
        MPI_Recv(filename_Buffer.data(), filename_Size, MPI_CHAR,
                 status.MPI_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::string filename_Recv(filename_Buffer.begin(),
                                  filename_Buffer.end());

        std::map<int, DecisionTree> forest_Recv =
            Serializer::deserializeMap<int, DecisionTree>(filename_Recv); 
        std::remove(filename_Recv.c_str());
        model.aggregate_Forest(forest_Recv);

        ++counter;
      }
    }
  } else {
    std::string filename = std::to_string(rank) + "_serialized_forest.bin";
    Serializer::serializeMap<int, DecisionTree>(model.get_Forest(), filename);

    MPI_Send(filename.c_str(), filename.size() + 1, MPI_CHAR, 0, 0,
             MPI_COMM_WORLD);
  } */