// -*- C++ -*-
/**
   
   @file opt/mpi/include/rarray.cc.h
   @brief RArray template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_rarray_cc_h__
#define __cxz_mpi_rarray_cc_h__

#include "rarray.h"
#include "raindex.h"

namespace CNORXZ
{
    namespace mpi
    {

	template <typename T>
	RCArray<T>::RCArray(const RCArray& a) :
	    mA(a.mA->copy()),
	    mGeom(a.mGeom),
	    mGlobal(a.mGlobal)
	{}

	template <typename T>
	RCArray<T>& RCArray<T>::operator=(const RCArray& a)
	{
	    mA = ObjHandle<CArrayBase<T>>(a.mA->copy());
	    mGeom = a.mGeom;
	    mGlobal = a.mGlobal;
	}

	template <typename T>
	RCArray<T>::RCArray(const CArrayBase<T>& a, const RangePtr& geom) :
	    mA(a.copy()),
	    mGeom(geom),
	    mGlobal(RRangeFactory(rangeCast<YRange>(a.range()),rangeCast<YRange>(mGeom)).create())
	{}

	template <typename T>
	template <typename I, typename M>
	T RCArray<T>::operator[](const IndexInterface<I,M>& i) const
	{
	    if constexpr(is_rank_index<I>::value){
		return *(begin() + i.lex());
	    }
	    else {
		return (*mA)[i];
	    }
	}

	template <typename T>
	template <typename I, typename M>
	T RCArray<T>::at(const IndexInterface<I,M>& i) const
	{
	    if constexpr(is_rank_index<I>::value){
		CXZ_ASSERT(i.rank() == getNumRanks(), "rank number out of scope");
		// consistency checks!!!
		return *(begin() + i.lex());
	    }
	    else {
		return mA->at(i);
	    }
	}

	template <typename T>
	template <class... Indices>
	T RCArray<T>::operator[](const SPack<Indices...>& pack) const
	{
	    CXZ_ERROR("not implemented");
	    return T();
	}

	template <typename T>
	template <class... Indices>
	T RCArray<T>::at(const SPack<Indices...>& pack) const
	{
	    CXZ_ERROR("not implemented");
	    return T();
	}

	template <typename T>
	T RCArray<T>::operator[](const DPack& pack) const
	{
	    CXZ_ERROR("not implemented");
	    return T();
	}

	template <typename T>
	T RCArray<T>::at(const DPack& pack) const
	{
	    CXZ_ERROR("not implemented");
	    return T();
	}

	template <typename T>
	template <typename I, typename M>
	Sptr<CArrayBase<T>> RCArray<T>::sl(const IndexInterface<I,M>& begin,
					   const IndexInterface<I,M>& end) const
	{
	    CXZ_ERROR("not implemented");
	    return nullptr;
	}

	template <typename T>
	template <class Index>
	COpRoot<T,Index> RCArray<T>::operator()(const Sptr<Index>& i) const
	{
	    CXZ_ERROR("not implemented");
	    return COpRoot<T,Index>();
	}

	template <typename T>
	template <class... Indices>
	inline decltype(auto) RCArray<T>::operator()(const SPack<Indices...>& pack) const
	{
	    CXZ_ERROR("not implemented");
	    //return COpRoot<T,Index>();
	    return 0;
	}

	template <typename T>
	inline decltype(auto) RCArray<T>::operator()(const DPack& pack) const
	{
	    CXZ_ERROR("not implemented");
	    //return COpRoot<T,Index>();
	    return 0;
	}

	template <typename T>
	const T* RCArray<T>::data() const
	{
	    return mA->data();
	}

	template <typename T>
	SizeT RCArray<T>::size() const
	{
	    return mA->size() * mGeom->size();
	}

	template <typename T>
	RangePtr RCArray<T>::range() const
	{
	    return mGlobal;
	}

	template <typename T>
	typename RCArray<T>::const_iterator RCArray<T>::begin() const
	{
	    return const_iterator(mA->data(), mGlobal);
	}

	template <typename T>
	typename RCArray<T>::const_iterator RCArray<T>::end() const
	{
	    return const_iterator(mA->data(), mGlobal, mGlobal->size());
	}

	template <typename T>
	typename RCArray<T>::const_iterator RCArray<T>::cbegin() const
	{
	    return const_iterator(mA->data(), mGlobal);
	}

	template <typename T>
	typename RCArray<T>::const_iterator RCArray<T>::cend() const
	{
	    return const_iterator(mA->data(), mGlobal, mGlobal->size());
	}

	template <typename T>
	bool RCArray<T>::isView() const
	{
	    return mA->isView();
	}

	template <typename T>
	const CArrayBase<T>& RCArray<T>::local() const
	{
	    return *mA;
	}

	template <typename T>
	RangePtr RCArray<T>::geom() const
	{
	    return mGeom;
	}
	
    } // namespace mpi
} // namespace CNORXZ

#endif
