#ifndef MPI_WRAPPER_H_
#define MPI_WRAPPER_H_

#include <stdio.h>
#include <string>

#include "boost/mpi.hpp"
#include <omp.h>

#include "Answers.hpp"
#include "BaggingModel.hpp"
#include "CrossValidation.hpp"
#include "DataSet.hpp"
//#include "Tools.hpp"

namespace MPI_Wrapper {

//
int balancer(int total_Elements, int num_Processes, int process_Rank) {
  int res = total_Elements / num_Processes;
  int remainder = total_Elements % num_Processes;

  return res + (process_Rank < remainder ? 1 : 0);
}

/*
 */
void MPI_Cross_Val(BaggingModel &model, const DataSet &data, int K) {

  // tools::display_Header(header);

  // local_Res.set_Header(header);

  // auto cross_Val_Res = CrossValidation::K_Folds(model, data, K);

  CrossValidation::K_Folds(model, data, K);


  // local_Res.set_Values(cross_Val_Res);
  //  Send && receive values from MPI Process to print in proc 0
  //  Only print once it has ended ppbly
  //  Serialize the values to display

  /* tools::display_Values("Fold n*" + std::to_string(i), depth, trees,
                        formatted_Matrix_Size, fold_Mae, fold_Mape,
                        fold_Std_Dev, t_Intern.get_Duration());

  tools::display_Values("Global", depth, trees, formatted_File_Size, global_MAE,
                        global_MAPE, global_Std_Dev, t_Global.get_Duration());
*/
}

//
void MPI_Main(int argc, char **argv) {

  pid_t pid = getpid();

  int rank, size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::string dataset_Path = argv[1];
  std::string metric = argv[2];
  std::string criteria = argv[3];
  int depth = std::stoi(argv[4]);
  int number_Of_Trees = std::atoi(argv[5]);

  int trees_For_Proc = balancer(number_Of_Trees, size, rank);

  BaggingModel model{metric, criteria, depth};

  DataSet DS{dataset_Path};

  Timer t;
  t.start();

  model.train(DS, trees_For_Proc);

  // CrossValidation::K_Folds(model, DS, 5);

  t.stop();

  std::cout << "Process infos : rank:= " << rank << " pid:= " << pid
            << " trees:= " << trees_For_Proc
            << " run time:= " << t.get_Duration() << "\n";

  // MPI_Barrier(MPI_COMM_WORLD);
  MPI_Cross_Val(model, DS, 5);
  /* if (rank == 0)
    CrossValidation::K_Folds(model, DS, 5); */
}

} // namespace MPI_Wrapper

#endif