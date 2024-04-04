#ifndef USER_HANDLER_H_
#define USER_HANDLER_H_

#include <cstdint>
#include <string>
#include <unordered_map>

class UserHandler {
  std::string dataset_Path;
  std::string metric;
  std::string criteria;

  uint16_t depth;
  uint16_t number_Of_Trees;
  uint16_t cross_Val_Folds;

  uint8_t cross_Val_Flag;

public:
  UserHandler();
  int handle_Command_Line(int argc, char **argv);

private:
  void helper_Printer();
  void config_Printer();

  uint16_t balancer(uint16_t total_Elements, uint16_t num_Processes,
                    int process_Rank);

  void input_Parser(int argc, char **argv, int rank, int size);
};

#endif