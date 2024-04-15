// -*- C++ -*-
/**
   
   @file opt/mpi/tests/mpi_env.h
   @brief MPI Environment.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_mpi_env_h__
#define __cxz_mpi_mpi_env_h__

#include <cstdlib>
#include "gtest/gtest.h"

namespace CNORXZ
{
    namespace mpi
    {
	class MPIEnv : public ::testing::Environment
	{
	public:

	    MPIEnv(int argc, char** argv) : mArgc(argc), mArgv(argv) {}
	
	    virtual ~MPIEnv() override {}

	    virtual void SetUp() override
	    {
		MPI_Init(&mArgc, &mArgv);
	    }

	    virtual void TearDown() override
	    {
		MPI_Finalize();
	    }

	protected:
	    int mArgc;
	    char** mArgv;
	};
    }
}

#endif
