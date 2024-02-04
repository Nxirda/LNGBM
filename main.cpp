#include "BaggingModel.hpp"
#include "CrossValidation.hpp"
#include "DataSet.hpp"

#include "EnumCriteria.hpp"
#include "EnumOperator.hpp"
#include "NodeSerializer.hpp"

#include "boost/mpi.hpp"

//#include "Validation.hpp"

// extern "C" {
#include <mpi.h>
//}
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

  /**************** TEMP
   * *******************************************************/
  // Create a TreeNode object with a tree structure
  /*

  TreeNode deserialized = Serializer::deserializeNode(filename);
  originalNode.node_Print_Criterion();
  std::cout << "\n";
  deserialized.node_Print_Criterion(); */
  /*****************************************************************************/

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  std::cout << "rank is : " << rank << "\n";
  std::cout << "size is : " << size << "\n";
  std::cout << "pid is  : " << pid << "\n";


  if ((argc != 6 || std::atoi(argv[5]) < size)&& rank == 0 ) {
    std::cout << "Usage is : mpiexec -np [Process] " << argv[0]
              << " [Path to DataSet] [Split Metric] [Split Criteria] [Depth] "
                 "[Number of Trees]\n";

    std::cout << "\n- Where process should be <= Number of Trees\n";

    std::cout << "\n== Split Metrics are ==\n";
    operators::print();

    std::cout << "\n== Split Criterias are ==\n";
    criterias::print();

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

  //MPI_Barrier(MPI_COMM_WORLD);

  // Get the result of the other processes to aggregate them
  if (rank == 0) {
    int filenameSize;

    MPI_Status status;

    MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_CHAR, &filenameSize);

    std::vector<char> filenameBuffer(filenameSize);

    MPI_Recv(filenameBuffer.data(), filenameSize, MPI_CHAR, 1, 0,
             MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    std::string filename_recv(filenameBuffer.begin(), filenameBuffer.end());

    TreeNode deserialized = Serializer::deserializeNode(filename_recv);
    deserialized.node_Print_Criterion(); 

    // Process the received file name
    std::cout << "Received file name: " << filename_recv << std::endl;

  } else {
    TreeNode originalNode(1, 0.5, 42.0);
    originalNode.add_Left(std::make_unique<TreeNode>(2, 0.3, 30.0));
    originalNode.add_Right(std::make_unique<TreeNode>(3, 0.7, 70.0));

    Serializer::serializeNode(originalNode, filename); 
    MPI_Send(filename.c_str(), filename.size() + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
  }

  std::cout << "Process with rank:= " << rank << " with pid:= " << pid
            << " finished training on " << trees_For_Proc << " Trees\n";
  // CrossValidation::K_Folds(model, DS, 5);

  MPI_Finalize();

  return 0;
}