// -*- C++ -*-
/**
   
   @file opt/mpi/include/raindex.cc.h
   @brief RAIndex template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_raindex_cc_h__
#define __cxz_mpi_raindex_cc_h__

#include <cstring>
#include "raindex.h"

namespace CNORXZ
{
    namespace mpi
    {
	template <typename T>
	RAIndex<T>::RAIndex(const T* loc, const RangePtr& range, SizeT lexpos) :
	    RIndex<YIndex,YIndex>(range, lexpos),
	    mLoc(loc),
	    mCur(rank()),
	    mThisRank(getRankNumber())
	{
	    setBufferSize();
	    setBuffer();
	}
	    

	template <typename T>
	RAIndex<T>::RAIndex(const T* loc, const RIndex<YIndex,YIndex>& i) :
	    RIndex<YIndex,YIndex>(i),
	    mLoc(loc),
	    mCur(rank()),
	    mThisRank(getRankNumber())
	{
	    setBufferSize();
	    setBuffer();
	}
	    

	template <typename T>
	RAIndex<T>::RAIndex(const T* loc, const RAIndex<T>& i) :
	    RAIndex(i)
	{
	    mLoc = loc;
	    setBuffer();
	}

	template <typename T>
	RAIndex<T> RAIndex<T>::operator+(Int n) const
	{
	    RAIndex<T> o = *this;
	    return o += n;
	}

	template <typename T>
	RAIndex<T> RAIndex<T>::operator-(Int n) const
	{
	    RAIndex<T> o = *this;
	    return o -= n;
	}

	template <typename T>
	const T& RAIndex<T>::operator*() const
	{
	    if(mCur != rank()){
		setBuffer();
	    }
	    if(rank() != mThisRank){
		return mBuf[local()->pos() % mBufSize];
	    }
	    else {
		return mLoc[local()->pos()];
	    }
	}

	template <typename T>
	const T* RAIndex<T>::operator->() const
	{
	    if(mCur != rank()){
		setBuffer();
	    }
	    if(rank() != mThisRank){
		return mBuf.data() + local()->pos() % mBufSize;
	    }
	    else {
		return mLoc + local()->pos();
	    }
	}
	
	template <typename T>
	void RAIndex<T>::setBufferSize()
	{
	    // use the contiguous stride's size as buffer size:
	    mBufSize = 1;
	    const auto& df = deepFormat();
	    for(SizeT i = 0; i != df.size(); ++i){
		if(df[i] == 1){
		    mBufSize = deepMax()[i];
		}
	    }
	}

	template <typename T>
	void RAIndex<T>::setBuffer() const
	{
	    if(mBuf.size() != mBufSize){
		mBuf.resize(mBufSize);
	    }
	    // A Bcast alternative with const pointer to source would be better...
	    const T* d = mLoc + (local()->pos() / mBufSize) * mBufSize;
	    std::memcpy(mBuf.data(), d, mBufSize*sizeof(T));
	    MPI_Bcast(mBuf.data(), mBufSize*sizeof(T), MPI_BYTE, static_cast<int>(rank()),
		      MPI_COMM_WORLD );
	    mCur = rank();
	}
	
    } // namespace mpi
} // namespace CNOXRZ

#endif
