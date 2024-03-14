// -*- C++ -*-

/**
   
   @file
   @brief

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include "mpi_base.h"

namespace CNORXZ
{
    namespace mpi
    {

	SizeT getRankNumber()
	{
	    int s;
	    const int ret = MPI_Comm_rank(MPI_COMM_WORLD, &s);
	    CXZ_ASSERT(ret == MPI_SUCCESS, "got mpi error = " << ret);
	    return static_cast<SizeT>(s);
	}

	SizeT getNumRanks()
	{
	    int s;
	    const int ret = MPI_Comm_size(MPI_COMM_WORLD, &s);
	    CXZ_ASSERT(ret == MPI_SUCCESS, "got mpi error = " << ret);
	    return static_cast<SizeT>(s);
	}
	
    } // namespace mpi
} // namespace CNORXZ

