#include <boost/mpi.hpp>
#include <regex>

#include "EnumCriteria.hpp"
#include "EnumOperator.hpp"

#include "MPIWrapper.hpp"
#include "UserHandler.hpp"

#include "Loggin.hpp"

//
UserHandler::UserHandler() {
  this->metric = "MAPE";
  this->criteria = "RV";

  this->depth = 5;
  this->number_Of_Trees = 5;

  this->algorithm_Flag = 's';
  this->bins = 256;
  this->cross_Val_Flag = 0;
  this->cross_Val_Folds = 0;
};

//
UserHandler::~UserHandler(){};

//
uint8_t UserHandler::is_Integer(const std::string &str) {
  for (char c : str) {
    if (!std::isdigit(c)) {
      return 0;
    }
  }
  return 1;
}

//
void UserHandler::parse_dataset_Path(const std::string &value) {
  this->dataset_Path = value;
}

//
void UserHandler::parse_Metric(const std::string &value) {
  this->metric = value;
}

//
void UserHandler::parse_Criteria(const std::string &value) {
  this->criteria = value;
}

/*
Only supported modes atm are standard and histograms
This isnt really elegant but it will do the trick
*/
void UserHandler::parse_Algorithm(const std::string &value, int rank) {
  if (value.length() == 1 && (value.at(0) == 'h' || value.at(0) == 's'))
    this->algorithm_Flag = value.at(0);
  else {
    if (rank == 0)
      /* std::cerr << " < Value after -alg should be a char\n"; */
      logging::ERROR("Value after -alg should be a char");
    exit(1);
  }
}

//
void UserHandler::parse_Bins(const std::string &value, int rank) {
  if (this->algorithm_Flag == 'h') {
    if (is_Integer(value)) {
      this->bins = std::stoi(value);
    } else {
      if (rank == 0) {
        logging::ERROR("Value after -b should be an integer");
      }
      exit(1);
    }
  } else {
    if (rank == 0) {
      logging::ERROR("Bins can be set only if the histogram algorithm is set");
    }
    exit(1);
  }
}

//
void UserHandler::parse_Depth(const std::string &value, int rank) {
  if (is_Integer(value)) {
    this->depth = std::stoi(value);
  } else {
    if (rank == 0)
      logging::ERROR("Value after -dt should be an integer");
    exit(1);
  }
}

//
void UserHandler::parse_Number_Of_Trees(const std::string &value, int rank) {

  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (is_Integer(value)) {
    this->number_Of_Trees =
        std::stoi(value); // balancer(std::stoi(value), size, rank);
  } else {
    if (rank == 0)
      logging::ERROR("Value after -nt should be an integer");
    exit(1);
  }
}

//
void UserHandler::parse_Cross_Val(const std::string &value, int rank) {
  if (is_Integer(value)) {
    this->cross_Val_Flag = 1;
    this->cross_Val_Folds = std::stoi(value);
  } else {
    if (rank == 0)
      logging::ERROR("Value after -cv should be an integer");
    exit(1);
  }
}

//
#define MIN_ARGS 2
#define MAX_ARGS 7

//
void UserHandler::input_Parser(int argc, char **argv, int rank, int size) {

  // If incorrect number of args
  if ((argc < MIN_ARGS || argc > MAX_ARGS) && rank == 0) {
    helper_Printer(argv[0]);
    if (size > 1) {
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
    exit(1);
  }

  // Suppose to be either dataset or helper commands
  first_Arg_Handler(argv, rank, size);

  std::smatch match;
  std::regex flag_regex("-([a-zA-Z]+)(?:=(\\S+))?");

  // Actual parsing
  uint8_t lim = std::min(argc, MAX_ARGS);
  for (uint8_t i = MIN_ARGS; i < lim; ++i) {
    std::string arg = argv[i];

    // std::smatch match;
    if (std::regex_match(arg, match, flag_regex)) {
      std::string flag = match[1].str();
      std::string value = match[2].str();

      if (!flag.compare("nt")) {
        parse_Number_Of_Trees(value, rank);

      } else if (!flag.compare("dt")) {
        parse_Depth(value, rank);

      } else if (!flag.compare("cv")) {
        parse_Cross_Val(value, rank);

      } else if (!flag.compare("o")) {
        parse_Metric(value);

      } else if (!flag.compare("c")) {
        parse_Criteria(value);

      } else if (!flag.compare("alg")) {
        parse_Algorithm(value, rank);

      } else if (!flag.compare("b")) {
        parse_Bins(value, rank);
      } else {
        std::string message = "Invalid argument " + arg;
        logging::ERROR(message.c_str());
        exit(1);
      }
    } else {
      std::string message = "Invalid argument " + arg;
      logging::ERROR(message.c_str());
      exit(1);
    }
  }

  if (this->number_Of_Trees < size && rank == 0) {
    logging::ERROR("MPI Processes should be <= to the number of Trees");
    if (size > 1) {
      MPI_Abort(MPI_COMM_WORLD, 1);
    }
    exit(1);
  }
}

//
void UserHandler::first_Arg_Handler(char **argv, int rank, int size) {

  // Argument is either dataset path, -list or -help (-h)
  std::smatch match;
  std::string arg = argv[1];
  std::regex flag_regex("-([a-zA-Z_]+)");

  if (std::regex_match(arg, match, flag_regex)) {
    if (!match[1].str().compare("list") && rank == 0) {
      flags_Printer();

    } else if ((!match[1].str().compare("help") ||
                !match[1].str().compare("h")) &&
               rank == 0) {
      helper_Printer(argv[0]);

    } else if (!match[1].str().compare("list_operators") && rank == 0) {
      operators::print();

    } else if (!match[1].str().compare("list_criterias") && rank == 0) {
      criterias::print();

    } else if (rank == 0) {
      if (size > 1 && rank == 0) {
        MPI_Abort(MPI_COMM_WORLD, 1);
      }
      std::string message = "Invalid argument " + arg;
      logging::ERROR(message.c_str());
      helper_Printer(argv[0]);
    }
    exit(1);
  } else {
    parse_dataset_Path(argv[1]);
  }
}

//
int UserHandler::command_Line_Handler(int argc, char **argv) {

  int rank, size;
  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  input_Parser(argc, argv, rank, size);

  config_Printer();
  // Option -alg= either s for standard or h for histogram
  DataSet data{this->dataset_Path};
  BaggingModel model;

  if (this->algorithm_Flag != 'h') {
    model = std::move(BaggingModel{this->metric, this->criteria, this->depth,
                                   this->number_Of_Trees});
  } else {
    model =
        std::move(BaggingModel{this->bins, this->depth, this->number_Of_Trees});
  }

  Timer t;
  t.start();

  MPI_Wrapper::MPI_Train(model, data);

  t.stop();

  process_Infos_Printer(rank, t);

  if (this->cross_Val_Flag) {
    MPI_Wrapper::MPI_Cross_Val(model, data, this->cross_Val_Folds);
  }

  MPI_Finalize();
  return 0;
}

//
void UserHandler::helper_Printer(std::string program_Name) {
  std::cout << "Usage is : mpiexec -np [Process] ./main "
            << "[-h] [Path to DataSet]\n";

  std::cout << "\n";
  std::cout << "> Run with " << program_Name
            << " -list for more informations on different parameters\n";
}

//
void UserHandler::flags_Printer() {
  std::cout << "List of different supported flags\n";

  std::cout << "\n === Trees Settings ===\n";
  std::cout << " > -nt=<integer> : set the number of Trees\n";
  std::cout << " > -dt=<integer> : set the depth of the Trees\n";

  std::cout << "\n === Splitting Algorithm ===\n";
  std::cout << " > -o=<string>   : set the operator\n";
  std::cout << " > -c=<string>   : set the criteria\n";

  std::cout << "\n === Training Algorithm ===\n";
  std::cout << " > -alg=<char>   : set the algorithm\n";

  std::cout << "\n === Cross Validation ===\n";
  std::cout << " > -cv=<integer> : set the number of Folds\n";

  std::cout << "\n === Other Flag ===\n";
  std::cout << " > -b=<integer>   : set the bins if used with -alg=h\n";
  std::cout << " > -list_operators: list the available operators\n";
  std::cout << " > -list_criterias: list the available operators\n";

  std::cout << std::endl;
}

//
void UserHandler::config_Printer() {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    std::cout << " === Running Configuration ===\n";
    std::cout << "\n";

    if (this->algorithm_Flag != 'h') {
      std::cout << "> Metric   : " << this->metric << "\n";
      std::cout << "> Criteria : " << this->criteria << "\n";
    } else {
      std::cout << "> Algorithm: " << this->algorithm_Flag << "\n";
      std::cout << "> Bins     : " << this->bins << "\n";
    }
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

//
void UserHandler::process_Infos_Printer(int rank, const Timer &t) {
  const pid_t pid = getpid();

  std::cout << "Process infos |rank:= " << rank << " |pid:= " << pid
            << " |trees:= " << this->number_Of_Trees
            << " |run time:= " << t.get_Duration() << "\n";
}
