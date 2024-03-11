#ifndef MPI_WRAPPER_H_
#define MPI_WRAPPER_H_

#include <stdio.h>
#include <string>

#include "boost/mpi.hpp"
#include <omp.h>

#include "BaggingModel.hpp"

namespace MPI_Wrapper {

//
uint16_t balancer(uint16_t total_Elements, uint16_t num_Processes,
                  int process_Rank);

//
void MPI_Cross_Val(const BaggingModel &model, const DataSet &data, int K);

//
void MPI_Main(int argc, char **argv);

} // namespace MPI_Wrapper

#endif