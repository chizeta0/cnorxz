// -*- C++ -*-
/**
   
   @file opt/mpi/include/raindex.h
   @brief RAIndex declarations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_raindex_h__
#define __cxz_mpi_raindex_h__

#include "cnorxz.h"

namespace CNORXZ
{
    namespace mpi
    {

	/** ****
	    Index for multi-ranked array.
	    This is meant to be a global index, its position must be equal on each rank,
	    otherwise deadlocks will occur. This kind of index exists for
	    completeness (global array element access), it should not be used in
	    operations that require high performance.
	    For local iterations (which is what one usually does) a YIndex is sufficient.
	    @tparam T data type.
	*/
	template <typename T>
	class RAIndex : public RIndex<YIndex,YIndex>
	{
	public:
	    typedef typename RIndex<YIndex,YIndex>::IB IB;

	    DEFAULT_MEMBERS(RAIndex);
	    RAIndex(const T* loc, const RangePtr& range, SizeT lexpos = 0);
	    RAIndex(const T* loc, const RIndex<YIndex,YIndex>& i);
	    RAIndex(const T* loc, const RAIndex<T>& i);

	    RAIndex operator+(Int n) const;
	    RAIndex operator-(Int n) const;

	    const T& operator*() const;
	    const T* operator->() const;

	    void setBufferSize();
	    
	private:

	    void setBuffer();
	    
	    const T* mLoc = nullptr;
	    Vector<T> mBuf; // used if iterating over content on different rank
	    SizeT mCur; // current rank in the buffer
	    SizeT mBufSize;
	    SizeT mThisRank;
	};

    } // namespace mpi
} // namespace CNOXRZ

#endif
