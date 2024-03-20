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
	    typedef RAIndex const_iterator;

	    DEFAULT_MEMBERS(RCArray);
	    
	    /** Construct from local array object.
		@param a Local array.
		@param geom Rank geometry.
	     */
	    RCArray(const Sptr<CArrayBase<T>> a, const RangePtr& geom);

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
	    
	private:
	    ObjHandle<CArrayBase<T>> mA;
	    RangePtr mGeom;
	    RangePtr mGlobal;
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
