#ifndef MPI_WRAPPER_H_
#define MPI_WRAPPER_H_

#include <iostream>
#include <stdio.h>
#include <string>

#include "boost/mpi.hpp"
#include <omp.h>

#include "BaggingModel.hpp"

namespace MPI_Wrapper {

//
void MPI_Train(BaggingModel &model, const DataSet &data);

//
void MPI_Cross_Val(const BaggingModel &model, const DataSet &data, int K);

//
uint16_t balancer(uint16_t total_Elements, uint16_t num_Processes,
                  int process_Rank);
} // namespace MPI_Wrapper

#endif