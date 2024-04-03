#include "MPIWrapper.hpp"

#include "EnumCriteria.hpp"
#include "EnumOperator.hpp"

#include "Answers.hpp"
#include "CrossValidation.hpp"
#include "DataSet.hpp"
#include "Serializer.hpp"
#include "Timer.hpp"

namespace MPI_Wrapper {

//
void helper_Print() {
  std::cout << "Usage is : mpiexec -np [Process] ./main "
            << "[Path to DataSet][Split Metric][Split Criteria][Depth]"
            << "[Number of Trees][Cross_Val][Folds]\n ";

  std::cout << "\n- Where process should be <= Number of Trees\n";

  std::cout << "\n== Split Metrics are ==\n";
  operators::print();

  std::cout << "\n== Split Criterias are ==\n";
  criterias::print();

  std::cout << "\nCross Val shall be 'CV' followed by the number of folds "
               "for activation\n";
}

//
uint16_t balancer(uint16_t total_Elements, uint16_t num_Processes,
                  int process_Rank) {
  uint16_t res = total_Elements / num_Processes;
  uint16_t remainder = total_Elements % num_Processes;

  return res + (process_Rank < remainder ? 1 : 0);
}

//
void MPI_Cross_Val(const BaggingModel &model, const DataSet &data, int K) {

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  BaggingModel model_To_Validate(model.get_Operator()->get_Name(),
                                 model.get_Criteria()->get_Name(),
                                 model.get_Depth(), model.get_Trees_Number());

  Answers cross_Val_Res = CrossValidation::K_Folds(model_To_Validate, data, K);

  //  Send && receive values from MPI Process to print in proc 0
  //  Only print once it has ended ppbly
  //  Serialize the values to display

  if (rank == 0) {
    int counter = 0;

    while (counter < size - 1) {
      int flag = 0;
      // Checks for incoming message
      MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &flag, MPI_STATUS_IGNORE);

      if (flag) {
        int recv_Size;
        MPI_Status status;

        // Get infos
        MPI_Probe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_CHAR, &recv_Size);

        std::vector<char> recv_Buffer(recv_Size);

        // Get message and handle infos
        MPI_Recv(recv_Buffer.data(), recv_Size, MPI_CHAR, status.MPI_SOURCE, 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::string recv_String(recv_Buffer.begin(), recv_Buffer.end());

        Answers recv_Answer = Serializer::deserialize_Answers(recv_String);
        cross_Val_Res.add_And_Mean_Values(recv_Answer);

        ++counter;
      }
    }
    cross_Val_Res.print();
  } else {
    std::string local_Answer = Serializer::serialize_Answers(cross_Val_Res);

    MPI_Send(local_Answer.c_str(), local_Answer.size(), MPI_CHAR, 0, 0,
             MPI_COMM_WORLD);
  }
}

//
BaggingModel MPI_Model_Init(std::string &metric, const std::string &criteria,
                            uint16_t depth, uint16_t number_Of_Trees) {
  int rank, size;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const uint16_t trees_For_Process = balancer(number_Of_Trees, size, rank);

  BaggingModel model{metric, criteria, depth, number_Of_Trees};

  return std::move(model);
}

//
void MPI_Train(BaggingModel &model, const DataSet &data) { model.train(data); }

//
int MPI_Main(int argc, char **argv) {

  const pid_t pid = getpid();

  int rank, size;
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (((argc != 8 && argc != 6) || std::atoi(argv[5]) < size) && rank == 0) {
    helper_Print();
    if (size > 1)
      MPI_Abort(MPI_COMM_WORLD, 1);

    return 1;
  }

  // For the model
  std::string dataset_Path = argv[1];
  std::string metric = argv[2];
  std::string criteria = argv[3];

  const uint16_t depth = std::stoi(argv[4]);
  const uint16_t number_Of_Trees = std::atoi(argv[5]);
  const uint16_t trees_For_Process = balancer(number_Of_Trees, size, rank);

  // BaggingModel model;

  DataSet data{dataset_Path};
  BaggingModel model{
      std::move(MPI_Model_Init(metric, criteria, depth, number_Of_Trees))};

  Timer t;
  t.start();

  // model.train(DS);
  MPI_Train(model, data);

  t.stop();

  std::cout << "Process infos |rank:= " << rank << " |pid:= " << pid
            << " |trees:= " << trees_For_Process
            << " |run time:= " << t.get_Duration() << "\n";

  if (argc == 8) {
    // For the cross validation
    std::string cross_Val = argv[6];
    const int folds = std::atoi(argv[7]);

    if (cross_Val.compare("CV"))
      return 1;

    MPI_Cross_Val(model, data, folds);
  }

  MPI_Finalize();
  return 0;
}

} // namespace MPI_Wrapper
