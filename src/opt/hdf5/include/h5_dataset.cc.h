// -*- C++ -*-
/**

   @file opt/hdf5/include/h5_dataset.cc.h
   @brief Implementation of template member functions of Dataset and SDataset.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_h5_dataset_cc_h__
#define __cxz_h5_dataset_cc_h__

#include "h5_dataset.h"

namespace CNORXZ
{
    namespace hdf5
    {
	template <typename T>
	Dataset& Dataset::init(const ArrayBase<T>& data)
	{
	    const hid_t tid = getTypeId(*data.data());
	    VCHECK(tid);
	    init(data.range(), tid);
	    if(data.begin().formatIsTrivial()){
		Vector<hsize_t> dims(mDataRange->dim());
		for(SizeT i = 0; i != dims.size(); ++i){
		    dims[i] = mDataRange->sub(i)->size();
		}
		const hid_t memspace = H5Screate_simple(dims.size(), dims.data(), NULL);
		H5Dwrite(mId, mType, memspace, mFilespace, H5P_DEFAULT, data.data());
		H5Sclose(memspace);
	    }
	    else {
		CXZ_ERROR("Got array type with non-trivial format; non-contiguous data formats are not supported yet!");
	    }
	    return *this;
	}

	template <typename T>
	SDataset<T>::SDataset(const String& name, const ContentBase* _parent) :
	    Dataset(name, _parent)
	{}

	template <typename T>
	MArray<T> SDataset<T>::read() const
	{
	    Vector<hsize_t> dims(mDataRange->dim());
	    for(SizeT i = 0; i != dims.size(); ++i){
		dims[i] = mDataRange->sub(i)->size();
	    }
	    const hid_t mem_space_id = H5Screate_simple(static_cast<hsize_t>(dims.size()),
							dims.data(), nullptr);
	    const hid_t xfer_plist_id = H5Pcreate(H5P_DATASET_XFER);
	    MArray<T> out(mDataRange);
	    const herr_t err = H5Dread(mId, mType, mem_space_id, mFilespace, xfer_plist_id, out.data());
	    CXZ_ASSERT(err >= 0, "error while reading dataset '" << mName
		       << "', errorcode :" << err);
	    H5Pclose(xfer_plist_id);
	    H5Sclose(mem_space_id);
	    return out;
	}

	template <typename T>
	template <class I, typename M>
	MArray<T> SDataset<T>::read(const IndexInterface<I,M>& beg, const IndexInterface<I,M>& end) const
	{
	    CXZ_ASSERT(beg.dim() == mDataRange->dim(), "got index of inconsistent dimension, got"
		       << beg.dim() << ", expected " << mDataRange->dim());
	    const RangePtr outrange = beg.prange(end);
	    Vector<hsize_t> dims(outrange->dim());
	    for(SizeT i = 0; i != dims.size(); ++i){
		dims[i] = outrange->sub(i)->size();
	    }
	    const Vector<hsize_t> fpos = mkFPos(beg);
	    H5Sselect_hyperslab(mFilespace, H5S_SELECT_SET, fpos.data(), NULL, dims.data(), NULL);
	    const hid_t mem_space_id = H5Screate_simple(static_cast<hsize_t>(dims.size()),
							dims.data(), nullptr);
	    const hid_t xfer_plist_id = H5Pcreate(H5P_DATASET_XFER);
	    MArray<T> out(outrange);
	    const herr_t err = H5Dread(mId, mType, mem_space_id, mFilespace, xfer_plist_id, out.data());
	    CXZ_ASSERT(err >= 0, "error while reading dataset '" << mName
		       << "', errorcode :" << err);
	    H5Pclose(xfer_plist_id);
	    H5Sclose(mem_space_id);
	    return out;
	}

	template <typename T>
	template <class I, typename M>
	Vector<hsize_t> SDataset<T>::mkFPos(const IndexInterface<I,M>& beg) const
	{
	    Vector<hsize_t> fpos(beg.dim());
	    if constexpr(has_static_sub<I>::value){
		iter<0,index_dim<I>::value> ( [&](auto i) { fpos[i] = beg.THIS().pack().get(i)->lex(); }, NoF{} );
	    }
	    else if constexpr(has_sub<I>::value){
		for(SizeT i = 0; i != beg.dim(); ++i){
		    fpos[i] = beg.THIS().pack().get(i)->lex();
		}
	    }
	    else {
		fpos[0] = beg.lex();
	    }
	    return fpos;
	}

    }
}

#endif
