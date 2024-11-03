// -*- C++ -*-
/**

   @file opt/hdf5/lib/h5_file.cc
   @brief RFile implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#include "h5_rfile.h"
#include "mpi.h"

namespace CNORXZ
{
    namespace hdf5
    {
	RFile::RFile(const String& fname, bool _ro) :
	    File(fname, _ro)
	{
	    mFAPL_id = H5Pcreate(H5P_FILE_ACCESS);
	    H5Pset_fapl_mpio(mFAPL_id, MPI_COMM_WORLD, MPI_INFO_NULL);
	}

	RFile::~RFile()
	{
	    this->close();
	    if(mFAPL_id){
		H5Pclose(mFAPL_id);
	    }
	}
    }
}
