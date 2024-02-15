#ifndef MPI_WRAPPER_H_
#define MPI_WRAPPER_H_

#include <stdio.h>
#include <string>

#include "boost/mpi.hpp"
#include <omp.h>


#include "BaggingModel.hpp"

namespace MPI_Wrapper {

//
int balancer(int total_Elements, int num_Processes, int process_Rank);

/*
 */
void MPI_Cross_Val(BaggingModel &model, const DataSet &data, int K);

//
void MPI_Main(int argc, char **argv);

} // namespace MPI_Wrapper

#endif