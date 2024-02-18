#include "MPIWrapper.hpp"

#include "Answers.hpp"
#include "CrossValidation.hpp"
#include "DataSet.hpp"
#include "Serializer.hpp"
#include "Timer.hpp"

namespace MPI_Wrapper {

/*
 */
int balancer(int total_Elements, int num_Processes, int process_Rank) {
  int res = total_Elements / num_Processes;
  int remainder = total_Elements % num_Processes;

  return res + (process_Rank < remainder ? 1 : 0);
}

/*
 */
void MPI_Cross_Val(BaggingModel &model, const DataSet &data, int K) {

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  Answers cross_Val_Res = CrossValidation::K_Folds(model, data, K);

  // local_Res.set_Values(cross_Val_Res);
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

        Answers answer_Recv = Serializer::deserialize_Answers(filename_Recv);
        std::remove(filename_Recv.c_str());
        cross_Val_Res.add_And_Mean_Values(answer_Recv);

        ++counter;
      }
    }
    cross_Val_Res.print();
  } else {
    std::string filename = std::to_string(rank) + "_serialized_answers.bin";
    Serializer::serialize_Answers(cross_Val_Res, filename);

    MPI_Send(filename.c_str(), filename.size() + 1, MPI_CHAR, 0, 0,
             MPI_COMM_WORLD);
  }
}

/*
 */
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

  t.stop();

  std::cout << "Process infos : rank:= " << rank << " pid:= " << pid
            << " trees:= " << trees_For_Proc
            << " run time:= " << t.get_Duration() << "\n";

  MPI_Cross_Val(model, DS, 5);   
}

} // namespace MPI_Wrapper
