#include "UserHandler.hpp"
#include <iostream>

/*********************/
/*                   */
/*      MAIN         */
/*                   */
/*********************/

//
int main(int argc, char **argv) {

  UserHandler handler{};
  int out = handler.handle_Command_Line(argc, argv);

  if (out == 1) {
    std::cerr << "\nProgram terminated with an error\n";
    return 1;
  }

  return out;
}