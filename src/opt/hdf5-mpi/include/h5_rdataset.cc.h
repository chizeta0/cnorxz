// -*- C++ -*-
/**

   @file opt/hdf5-mpi/include/h5_dataset.cc.h
   @brief Implementation of template member functions of RDataset and SRDataset.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_h5_rdataset_cc_h__
#define __cxz_h5_rdataset_cc_h__

#include "h5_rdataset.h"

namespace CNORXZ
{
    namespace hdf5
    {
	template <typename T>
	RDataset& RDataset::init(const mpi::RArray<T>& data)
	{
	    const hid_t tid = getTypeId(*data.data());
	    if(data.begin().formatIsTrivial()){
		dynamic_cast<Dataset*>(this)->initbase(data.range(), tid, data.data());
	    }
	    else {
		CXZ_ERROR("Got array type with non-trivial format; non-contiguous data formats are not supported yet!");
	    }
	    return *this;
	}
	
	template <typename T>
	SRDataset<T>::SRDataset(const String& name, const ContentBase* _parent) :
	    RDataset(name, _parent)
	{}

	template <typename T>
	mpi::RArray<T> SRDataset<T>::read(const RangePtr& geom) const
	{
	    auto rr = rangeCast<mpi::RRange<YRange,YRange>>( mpi::rrange(mFileRange, geom) );
	    mpi::RArray<T> out(rr);
	    readbase(out.data(), rr, nullptr);
	    return out;
	}
	
    }
}
    
#endif
