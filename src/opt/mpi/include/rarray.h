// -*- C++ -*-
/**
   
   @file opt/mpi/include/rarray.h
   @brief RArray declarations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_rarray_h__
#define __cxz_mpi_rarray_h__

#include "cnorxz.h"
#include "raindex.h"

namespace CNORXZ
{
    namespace mpi
    {
	/** ****
	    Wrapper for cnorxz array types for handling MPI ranks.
	    @tparam T data type.
	*/
	template <typename T>
	class RCArray
	{
	public:
	    typedef RAIndex<T> const_iterator;

	    DEFAULT_C(RCArray);
	    DEFAULT_MOVE(RCArray);
	    RCArray(const RCArray& a);
	    RCArray& operator=(const RCArray& a);
	    
	    /** Construct from local array object.
		@param a Local array.
		@param geom Rank geometry.
	     */
	    RCArray(const CArrayBase<T>& a, const RangePtr& geom);

	    /** @copydoc CArrayBase::operator[] */
	    template <typename I, typename M>
	    T operator[](const IndexInterface<I,M>& i) const;

	    /** @copydoc CArrayBase::at()
		Return copy instead of reference.
	     */
	    template <typename I, typename M>
	    T at(const IndexInterface<I,M>& i) const;

	    /** @copydoc CArrayBase::operator[] */
	    template <class... Indices>
	    T operator[](const SPack<Indices...>& pack) const;

	    /** @copydoc CArrayBase::at() */
	    template <class... Indices>
	    T at(const SPack<Indices...>& pack) const;

	    /** @copydoc CArrayBase::operator[] */
	    T operator[](const DPack& pack) const;

	    /** @copydoc CArrayBase::at() */
	    T at(const DPack& pack) const;

	    /** @copydoc CArrayBase::sl() */
	    template <typename I, typename M>
	    Sptr<CArrayBase<T>> sl(const IndexInterface<I,M>& begin,
				   const IndexInterface<I,M>& end) const;

	    /** @copydoc CArrayBase::operator() */
	    template <class Index>
	    COpRoot<T,Index> operator()(const Sptr<Index>& i) const;

	    /** @copydoc CArrayBase::operator() */
	    template <class... Indices>
	    inline decltype(auto) operator()(const SPack<Indices...>& pack) const;

	    /** @copydoc CArrayBase::operator() */
	    inline decltype(auto) operator()(const DPack& pack) const;

	    /** @copydoc CArrayBase::data() */
	    const T* data() const;

	    /** @copydoc CArrayBase::size() */
	    SizeT size() const;

	    /** @copydoc CArrayBase:range() */
	    RangePtr range() const;

	    /** @copydoc CArrayBase::begin() */
	    const_iterator begin() const;

	    /** @copydoc CArrayBase::end() */
	    const_iterator end() const;

	    /** @copydoc CArrayBase::cbegin() */
	    const_iterator cbegin() const;

	    /** @copydoc CArrayBase::cend() */
	    const_iterator cend() const;

	    /** @copydoc CArrayBase::isView() */
	    bool isView() const;

	    /** Get local array object. */
	    const CArrayBase<T>& local() const;

	    /** Get rank geometry. */
	    RangePtr geom() const;

	    /** Load all data from other ranks that is accessed by i2 in a loop over i1.
		@param i1 Loop index.
		@param i2 Access index.
	     */
	    template <class Index1, class Index2>
	    void load(const Sptr<Index1>& i1, const Sptr<Index2>& i2) const;
	    
	private:
	    ObjHandle<CArrayBase<T>> mA;
	    RangePtr mGeom;
	    RangePtr mGlobal;
	    mutable Vector<T> mBuf; // data from other ranks
	    mutable Vector<const T*> mMap; // point to positions in mA or mBuf
	};

	/** ****
	    Wrapper for cnorxz array types for handling MPI ranks.
	    @tparam T data type.
	*/
	template <typename T>
	class RArray : public RCArray<T>
	{
	public:
	    typedef RCArray<T> RCA;
	
	    using RCA::operator[];
	    using RCA::operator();
	    using RCA::at;
	    using RCA::data;
	    using RCA::begin;
	    using RCA::end;
	    using RCA::cbegin;
	    using RCA::cend;
	    using RCA::sl;

	    DEFAULT_MEMBERS(RArray);

	
	};
    } // namespace mpi
} // namespace CNORXZ

#endif
