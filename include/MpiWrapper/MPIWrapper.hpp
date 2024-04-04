#ifndef MPI_WRAPPER_H_
#define MPI_WRAPPER_H_

#include <stdio.h>
#include <string>
#include <iostream>

#include "boost/mpi.hpp"
#include <omp.h>

#include "BaggingModel.hpp"

namespace MPI_Wrapper {

//
void MPI_Train(BaggingModel &model, const DataSet &data);

//
void MPI_Cross_Val(const BaggingModel &model, const DataSet &data, int K);

} // namespace MPI_Wrapper

#endif