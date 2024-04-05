#ifndef USER_HANDLER_H_
#define USER_HANDLER_H_

#include <Timer.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>

class UserHandler {
private:
  std::string dataset_Path;
  std::string metric;
  std::string criteria;

  uint16_t depth;
  uint16_t number_Of_Trees;
  uint16_t cross_Val_Folds;

  uint8_t cross_Val_Flag;

public:
  UserHandler();
  ~UserHandler();
  int command_Line_Handler(int argc, char **argv);

private:
  void parse_Metric(const std::string &value);
  void parse_Criteria(const std::string &value);
  void parse_dataset_Path(const std::string &value);
  void parse_Depth(const std::string &value, int rank);
  void parse_Cross_Val(const std::string &value, int rank);
  void parse_Number_Of_Trees(const std::string &value, int rank);

  void flags_Printer();
  void config_Printer();
  void process_Infos_Printer(int rank, const Timer &t);
  void helper_Printer(std::string program_Name);

  uint16_t balancer(uint16_t total_Elements, uint16_t num_Processes,
                    int process_Rank);

  uint8_t is_Integer(const std::string &str);

  void first_Arg_Handler(char **argv, int rank, int size);

  void input_Parser(int argc, char **argv, int rank, int size);
};

#endif