#include "BaggingModel.hpp"
#include "CrossValidation.hpp"
#include "DataSet.hpp"

#include "EnumCriteria.hpp"
#include "EnumOperator.hpp"
#include "Serializer.hpp"

#include "boost/mpi.hpp"

#include <stdio.h>

using namespace std;

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
int main(int argc, char **argv) {

  int rank, size;
  MPI_Init(&argc, &argv);
  pid_t pid = getpid();
  std::string filename = "serialized_tree_node.bin";

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
        int filenameSize;
        MPI_Status status;

        // Get infos
        MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_CHAR, &filenameSize);

        std::vector<char> filenameBuffer(filenameSize);

        // Get message and handle infos
        MPI_Recv(filenameBuffer.data(), filenameSize, MPI_CHAR,
                 status.MPI_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::string filename_recv(filenameBuffer.begin(), filenameBuffer.end());

        std::map<int, DecisionTree> res = Serializer::deserializeMap<int, DecisionTree>(filename_recv);
        model.aggregate_Forest(res);

        ++counter;
      }
    }
    std::cout << "Total number of trees is : " << model.get_Forest().size() << "\n";
  } else {
    Serializer::serializeMap<int, DecisionTree>(model.get_Forest(), filename);
    MPI_Send(filename.c_str(), filename.size() + 1, MPI_CHAR, 0, 0,
             MPI_COMM_WORLD);
  }

  std::cout << "Process with rank:= " << rank << " with pid:= " << pid
            << " finished training on " << trees_For_Proc << " Trees\n";

  // CrossValidation::K_Folds(model, DS, 5);

  MPI_Finalize();

  return 0;
}