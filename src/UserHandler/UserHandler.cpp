#include <boost/mpi.hpp>
#include <regex>

#include "EnumCriteria.hpp"
#include "EnumOperator.hpp"

#include "MPIWrapper.hpp"
#include "Timer.hpp"
#include "UserHandler.hpp"

#define MIN_ARGS 2
#define MAX_ARGS 7

//
UserHandler::UserHandler() {
  this->metric = "MAPE";
  this->criteria = "RV";

  this->depth = 5;
  this->number_Of_Trees = 5;

  this->cross_Val_Flag = 0;
  this->cross_Val_Folds = 0;
};

//
uint8_t is_Unsigned_Integer(const std::string &str) {
  for (char c : str) {
    if (!std::isdigit(c)) {
      return 0;
    }
  }
  return 1;
}

//
void UserHandler::input_Parser(int argc, char **argv, int rank, int size) {

  if ((argc < MIN_ARGS || argc > MAX_ARGS) && rank == 0) {
    helper_Printer();
    if (size > 1) {
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
    exit(1);
  }

  this->dataset_Path = argv[1];

  std::regex flag_regex("-([a-zA-Z]+)(?:=(\\S+))?");

  std::string arg;
  uint8_t lim = std::min(argc, MAX_ARGS);
  for (uint8_t i = MIN_ARGS; i < lim; ++i) {
    arg = argv[i];

    std::smatch match;
    if (std::regex_match(arg, match, flag_regex)) {
      std::string flag = match[1].str();
      std::string value = match[2].str();

      if (!flag.compare("nt")) {
        if (is_Unsigned_Integer(value)) {
          this->number_Of_Trees = balancer(std::stoi(value), size, rank);
        } else {
          std::cerr << " < Value after -nt shoul'd be an integer\n";
          exit(1);
        }

      } else if (!flag.compare("dt")) {
        if (is_Unsigned_Integer(value)) {
          this->depth = std::stoi(value);
        } else {
          std::cerr << " < Value after -dt shoul'd be an integer\n";
          exit(1);
        }

      } else if (!flag.compare("cv")) {
        if (is_Unsigned_Integer(value)) {
          this->cross_Val_Flag = 1;
          this->cross_Val_Folds = std::stoi(value);
        } else {
          std::cerr << " < Value after -cv shoul'd be an integer\n";
          exit(1);
        }

      } else if (!flag.compare("o")) {
        this->metric = value;
      } else if (!flag.compare("c")) {
        this->criteria = criteria;
      }
    }
  }
}

//
int UserHandler::handle_Command_Line(int argc, char **argv) {
  const pid_t pid = getpid();

  int rank, size;
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  input_Parser(argc, argv, rank, size);

  config_Printer();

  DataSet data{this->dataset_Path};
  BaggingModel model{std::move(BaggingModel(
      this->metric, this->criteria, this->depth, this->number_Of_Trees))};

  Timer t;
  t.start();

  // model.train(DS);
  MPI_Wrapper::MPI_Train(model, data);

  t.stop();

  std::cout << "Process infos |rank:= " << rank << " |pid:= " << pid
            << " |trees:= " << this->number_Of_Trees
            << " |run time:= " << t.get_Duration() << "\n";

  if (this->cross_Val_Flag) {
    MPI_Wrapper::MPI_Cross_Val(model, data, this->cross_Val_Folds);
  }

  MPI_Finalize();
  return 0;
}

//
uint16_t UserHandler::balancer(uint16_t total_Elements, uint16_t num_Processes,
                               int process_Rank) {
  uint16_t res = total_Elements / num_Processes;
  uint16_t remainder = total_Elements % num_Processes;

  return res + (process_Rank < remainder ? 1 : 0);
}

//
void UserHandler::helper_Printer() {
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
void UserHandler::config_Printer() {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    std::cout << " === Running Configuration ===\n";
    std::cout << "\n";
    std::cout << "> Metric   : " << this->metric << "\n";
    std::cout << "> Criteria : " << this->criteria << "\n";
    std::cout << "> Depth    : " << this->depth << "\n";
    std::cout << "> Trees    : " << this->number_Of_Trees << "\n";
    if (this->cross_Val_Flag) {
      std::cout << "> CV       : yes\n";
      std::cout << "> Folds    : " << this->cross_Val_Folds << "\n";
    } else {
      std::cout << "> CV       : no\n";
    }
    std::cout << "\n";
  }
}
