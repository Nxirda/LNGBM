#include "BaggingModel.hpp"
#include "CrossValidation.hpp"
#include "DataSet.hpp"

#include "EnumCriteria.hpp"
#include "EnumOperator.hpp"
#include "Serializer.hpp"
#include "Timer.hpp"

#include <omp.h>
#include "boost/mpi.hpp"

#include <cstdio>
#include <stdio.h>

/*********************/
/*                   */
/*    TEMPORARY      */
/*                   */
/*********************/

//
int balancer(int total_Elements, int num_Processes, int process_Rank) {
  int res = total_Elements / num_Processes;
  int remainder = total_Elements % num_Processes;

  return res + (process_Rank < remainder ? 1 : 0);
}

//
void MPI_Handler(int argc, char **argv, int rank, int size) {

  Timer t;
  t.start();

  pid_t pid = getpid();
  std::string dataset_Path = argv[1];
  std::string metric = argv[2];
  std::string criteria = argv[3];
  int depth = std::stoi(argv[4]);
  int number_Of_Trees = std::atoi(argv[5]);

  int trees_For_Proc = balancer(number_Of_Trees, size, rank);

  BaggingModel model{metric, criteria, depth};

  DataSet DS{dataset_Path};

  model.train(DS, trees_For_Proc);

  // Get the result of the other processes to aggregate them
  if (rank == 0) {
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
  }

  t.stop();

  std::cout << "Process infos : rank:= " << rank << " pid:= " << pid
            << " trees:= " << trees_For_Proc << " run time:= "<< t.get_Duration() <<"\n";

  //MPI_Barrier(MPI_COMM_WORLD);

  /* if (rank == 0)
    CrossValidation::K_Folds(model, DS, 5); */
}

//
int main(int argc, char **argv) {

  int rank, size;
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if ((argc != 6 || std::atoi(argv[5]) < size) && rank == 0) {
    std::cout << "Usage is : mpiexec -np [Process] " << argv[0]
              << " [Path to DataSet] [Split Metric] [Split Criteria] [Depth] "
                 "[Number of Trees]\n";

    std::cout << "\n- Where process should be <= Number of Trees\n";

    std::cout << "\n== Split Metrics are ==\n";
    operators::print();

    std::cout << "\n== Split Criterias are ==\n";
    criterias::print();

    if (size > 0)
      MPI_Abort(MPI_COMM_WORLD, 1);

    return 1;
  }

  #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        #pragma omp critical
        std::cout << "Thread ID: " << thread_id << " Hello, OpenMP!" << std::endl;
    }

  MPI_Handler(argc, argv, rank, size);

  MPI_Finalize();

  return 0;
}