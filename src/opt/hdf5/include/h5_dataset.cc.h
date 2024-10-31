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
	    if(data.begin().formatIsTrivial()){
		initbase(data.range(), tid, data.data());
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
	    MArray<T> out(mFileRange);
	    readbase(out.data(), nullptr, nullptr);
	    return out;
	}

	template <typename T>
	template <class I, typename M>
	MArray<T> SDataset<T>::read(const IndexInterface<I,M>& idx) const
	{
	    CXZ_ASSERT(idx.dim() == mFileRange->dim(), "got index of inconsistent dimension, got"
		       << idx.dim() << ", expected " << mFileRange->dim());
	    const RangePtr outrange = idx.range();
	    CXZ_ASSERT(outrange->size() == mFileRange->size(),
		       "got index of range of inconsistent size, expected "
		       << mFileRange->size() << ", got " << outrange->size());
	    MArray<T> out(outrange);
	    readbase(out.data(), outrange, nullptr);
	    return out;
	}

	template <typename T>
	template <class I, typename M>
	MArray<T> SDataset<T>::read(const IndexInterface<I,M>& beg, const IndexInterface<I,M>& end) const
	{
	    CXZ_ASSERT(beg.dim() == mFileRange->dim(), "got index of inconsistent dimension, got"
		       << beg.dim() << ", expected " << mFileRange->dim());
	    const RangePtr outrange = beg.prange(end);
	    MArray<T> out(outrange);
	    readbase(out.data(), outrange, toYptr(beg));
	    return out;
	}

	template <typename T>
	template <class I, typename M>
	Sptr<YIndex> SDataset<T>::toYptr(const IndexInterface<I,M>& beg) const
	{
	    if constexpr(std::is_same<YIndex,I>::value){
		return std::make_shared<YIndex>(beg.THIS());
	    }
	    else if constexpr(has_sub<I>::value) {
		return yindexPtr(beg.pack());
	    }
	    else {
		return yindexPtr(DPack(std::make_shared<I>(beg)));
	    }
	}
    }
}

#endif
