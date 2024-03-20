// -*- C++ -*-
/**
   
   @file opt/mpi/include/raindex.cc.h
   @brief RAIndex template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_raindex_cc_h__
#define __cxz_mpi_raindex_cc_h__

namespace CNOXRZ
{
    namespace mpi
    {
	template <typename T>
	RAIndex<T>::RAIndex(const T* loc, const RangePtr& range, SizeT lexpos) :
	    RIndex<YIndex,YIndex>(range, lexpos),
	    mLoc(loc),
	    mCur(i.rank()),
	    mThisRank(getRankNumber())
	{
	    setBufferSize();
	    setBuffer();
	}
	    

	template <typename T>
	RAIndex<T>::RAIndex(const T* loc, const RIndex<YIndex,YIndex>& i) :
	    RIndex<YIndex,YIndex>(i)
	    mLoc(loc),
	    mCur(i.rank()),
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
	    if(rank() != mThisRank){
		if(mCur != rank()){
		    setBuffer();
		}
		return mBuf[local()->pos() % mBufSize];
	    }
	    else {
		mLoc[local()->pos()];
	    }
	}

	template <typename T>
	const T* RAIndex<T>::operator->() const
	{
	    if(rank() != mThisRank){
		if(mCur != rank()){
		    setBuffer();
		}
		return mBuf + local()->pos() % mBufSize;
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
	void RAIndex<T>::setBuffer()
	{
	    if(mBuf.size() != mBufSize){
		mBuf.resize(mBufSize);
	    }
	    // A Bcast alternative with const pointer to source would be better...
	    std::memcpy(mBuf.data(), mLoc + local()->pos() / mBufSize, mBufSize*sizeof(T));
	    MPI_Bcast(mBuf.data(), mBufSize*sizeof(T), MPI_BYTE, static_cast<int>(rank()),
		      MPI_COMM_WORLD );
	}
	
    } // namespace mpi
} // namespace CNOXRZ
