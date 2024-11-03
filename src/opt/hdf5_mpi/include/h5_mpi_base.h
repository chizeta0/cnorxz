
#ifndef __cxz_h5_mpi_base_h__
#define __cxz_h5_mpi_base_h__

#define CXZ_H5_MPI_BUILD_MODE 1

#if CXZ_H5_MPI_BUILD_MODE

#include "cnorxz_mpi.h"
#include "cnorxz_hdf5.h"

#else

#include "mpi/cnorxz_mpi.h"
#include "hdf5/cnorxz_hdf5.h"

#endif

#endif
