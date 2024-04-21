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

	    /** Get the buffer map.
		buffermap()[r (ranked)][i] returns the same as
		loc[r * i] if 'loc' is a purely local array with the same content.
		The buffer needs to be initialized before according to required range.
	     */
	    const Vector<const T*>& buffermap() const;
	    
	    /** Load all data from other ranks that is accessed by ai in a loop over lpi.
		imap indicates the global (!) position of ai for a given (local) position of lpi.
		@param lpi Loop rank index.
		@param ai Access rank index.
		@param i Index for non-ranked dimensions.
		@param imap Index position map.
	     */
	    template <class TarI, class RTarI, class SrcI, class RSrcI, class I>
	    void load(const Sptr<RIndex<TarI,RTarI>>& lpi, const Sptr<RIndex<SrcI,RSrcI>>& ai,
		      const Sptr<I>& i, const Sptr<Vector<SizeT>>& imap) const;

	    /** Load all data from other ranks that is accessed by f(i).
		@param lpi Loop index.
		@param f Map function.
		@return Index position map.
	     */
	    template <class Index, class F>
	    Sptr<Vector<SizeT>> load(const Sptr<Index>& lpi, const F& f) const;

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

	template <class TarI, class RTarI, class SrcI, class RSrcI, typename T>
	void setupBuffer(const Sptr<RIndex<TarI,RTarI>>& rgj, const Sptr<RIndex<SrcI,RSrcI>>& rgi,
			 const Sptr<Vector<SizeT>>& imap, const CArrayBase<T>& data,
			 Vector<T>& buf, Vector<const T*>& map, const SizeT blocks);

	
    } // namespace mpi
} // namespace CNORXZ

#endif
