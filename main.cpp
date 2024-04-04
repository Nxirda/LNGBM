#include "MPIWrapper.hpp"


/*********************/
/*                   */
/*      MAIN         */
/*                   */
/*********************/

//
int main(int argc, char **argv) {
  
  int out = MPI_Wrapper::MPI_Main(argc, argv);
  
  if(out == 1){
    std::cerr << "\nProgram terminated with an error\n";
    return 1;
  }

  return out;
}