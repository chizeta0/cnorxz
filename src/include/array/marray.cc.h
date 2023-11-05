// -*- C++ -*-
/**
   
   @file include/array/marray.cc.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_marray_cc_h__
#define __cxz_marray_cc_h__

#include "marray.h"

namespace CNORXZ
{
    /****************
     *    MArray    *
     ***************/
    
    template <typename T>
    MArray<T>::MArray(const RangePtr& range) :
	ArrayBase<T>(range), mCont(range->size())
    {}

    template <typename T>
    MArray<T>::MArray(const RangePtr& range, const Vector<T>& vec) :
	ArrayBase<T>(range), mCont(vec)
    {}

    template <typename T>
    MArray<T>::MArray(const RangePtr& range, Vector<T>&& vec) :
	ArrayBase<T>(range), mCont(vec)
    {}

    template <typename T>
    MArray<T>& MArray<T>::init(const RangePtr& range)
    {
	AB::mRange = rangeCast<YRange>(range);
	mCont.resize(AB::mRange->size());
	return *this;
    }

    template <typename T>
    MArray<T>& MArray<T>::extend(const RangePtr& range)
    {
	if(AB::mRange == nullptr) {
	    return this->init(range);
	}
	MArray<T> tmp(AB::mRange->extend(range));
	auto ei = this->end();
	auto ti = tmp.begin();
	// this is not very efficient; remove by sub-index operation once available:
	for(auto ii = this->begin(); ii != ei; ++ii){
	    ti.at(ii.meta());
	    *ti = *ii;
	}
	*this = std::move(tmp);
	return *this;
    }

    template <typename T>
    const T* MArray<T>::data() const
    {
	return mCont.data();
    }

    template <typename T>
    T* MArray<T>::data()
    {
	return mCont.data();
    }

    template <typename T>
    typename MArray<T>::const_iterator MArray<T>::cbegin() const
    {
	return const_iterator(mCont.data(), AB::mRange);
    }

    template <typename T>
    typename MArray<T>::const_iterator MArray<T>::cend() const
    {
	return const_iterator(mCont.data(), AB::mRange, mCont.size());
    }

    template <typename T>
    bool MArray<T>::isView() const
    {
	return false;
    }
    
}

#endif
