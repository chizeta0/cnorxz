// -*- C++ -*-
/**
   
   @file include/array/aindex.cc.h
   @brief Array index template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_aindex_cc_h__
#define __cxz_aindex_cc_h__

#include "aindex.h"

namespace CNORXZ
{

    template <typename T>
    AIndex<T>::AIndex(const T* data, const RangePtr& range, SizeT lexpos) :
	YIndex(range, lexpos),
	mCData(data)
    {}

    template <typename T>
    AIndex<T>::AIndex(const T* data, const YIndex& yindex) :
	YIndex(yindex),
	mCData(data)
    {}

    template <typename T>
    AIndex<T>::AIndex(const T* data, const AIndex<T>& aindex) :
	AIndex<T>(aindex)
    {
	mCData = data;
    }

    template <typename T>
    AIndex<T> AIndex<T>::operator+(Int n) const
    {
	AIndex<T> o = *this;
	o += n;
	return o;
    }

    template <typename T>
    AIndex<T> AIndex<T>::operator-(Int n) const
    {
	AIndex<T> o = *this;
	o -= n;
	return o;
    }

    template <typename T>
    const T& AIndex<T>::operator*() const
    {
	return mCData[IB::mPos];
    }
    
    template <typename T>
    const T* AIndex<T>::operator->() const
    {
	return mCData + IB::mPos;
    }

    template <typename T>
    BIndex<T>::BIndex(T* data, const RangePtr& range, SizeT lexpos) :
	AIndex<T>(data, range, lexpos),
	mData(data)
    {}

    template <typename T>
    BIndex<T>::BIndex(T* data, const AIndex<T>& ai) :
	AIndex<T>(data, ai),
	mData(data)
    {}

    template <typename T>
    BIndex<T> BIndex<T>::operator+(Int n) const
    {
	BIndex<T> o = *this;
	o += n;
	return o;
    }
    
    template <typename T>
    BIndex<T> BIndex<T>::operator-(Int n) const
    {
	BIndex<T> o = *this;
	o -= n;
	return o;
    }

    template <typename T>
    T& BIndex<T>::operator*()
    {
	return mData[IB::mPos];
    }

    template <typename T>
    T* BIndex<T>::operator->()
    {
	return mData + IB::mPos;
    }

}

#endif
