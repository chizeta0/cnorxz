// -*- C++ -*-
/**

   @file opt/hdf5_mpi/include/h5_mpi_utils.h
   @brief CNORXZ utilities for HDF5 together with MPI.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_h5_mpi_utils_h__
#define __cxz_h5_mpi_utils_h__

#include "h5_group.h"
#include "h5_rdataset.h"

namespace CNORXZ
{
    namespace hdf5
    {

	template <typename T>
	Sptr<SRDataset<T>> getRDataset(Group& group, const String& name);

	template <typename T>
	Group& addRDataset(Group& group, const String& name, const mpi::RArray<T>& data);

    }
}

#endif
