// -*- C++ -*-
/**

   @file opt/hdf5_mpi/include/h5_mpi_utils.cc.h
   @brief CNORXZ utilities for HDF5 together with MPI.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_h5_mpi_utils_cc_h__
#define __cxz_h5_mpi_utils_cc_h__

#include "h5_mpi_utils.h"

namespace CNORXZ
{
    namespace hdf5
    {
	template <typename T>
	Sptr<SRDataset<T>> getRDataset(Group& group, const String& name)
	{
	    return group.get(name, [](const String& name_, const ContentBase* par, auto& i) {
		(*i)->close();
		auto dset = std::make_shared<SRDataset<Double>>(name_, par);
		*i = dset;
		return dset;
	    } );
	}

	template <typename T>
	Group& addRDataset(Group& group, const String& name, const mpi::RArray<T>& data)
	{
	    return group.add("dat", [](const String& name_, const ContentBase* par,
				       const mpi::RArray<Double>& d) {
		auto o = std::make_shared<SRDataset<Double>>( name_, par );
		o->init(d);
		return o;
	    }, data );
	}
	
    }
}

#endif
