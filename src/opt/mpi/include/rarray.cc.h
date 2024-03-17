// -*- C++ -*-
/**
   
   @file opt/mpi/include/rarray.cc.h
   @brief RArray template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_rarray_cc_h__
#define __cxz_mpi_rarray_cc_h__

namespace CNORXZ
{
    namespace mpi
    {

	template <typename T>
	RCArray<T>::RCArray(const Sptr<CArrayBase<T>> a, const RangePtr& geom) :
	    mA(a),
	    mGeom(geom)
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
	}

	template <typename T>
	template <class... Indices>
	T RCArray<T>::at(const SPack<Indices...>& pack) const
	{
	}

	template <typename T>
	T RCArray<T>::operator[](const DPack& pack) const
	{
	}

	template <typename T>
	T RCArray<T>::at(const DPack& pack) const
	{
	}

	template <typename T>
	template <typename I, typename M>
	Sptr<CArrayBase<T>> RCArray<T>::sl(const IndexInterface<I,M>& begin,
					   const IndexInterface<I,M>& end) const
	{
	}

	template <typename T>
	template <class Index>
	COpRoot<T,Index> RCArray<T>::operator()(const Sptr<Index>& i) const
	{
	}

	template <typename T>
	template <class... Indices>
	inline decltype(auto) RCArray<T>::operator()(const SPack<Indices...>& pack) const
	{
	}

	template <typename T>
	inline decltype(auto) RCArray<T>::operator()(const DPack& pack) const
	{
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
	    return RRangeFactory(mA->range(),mGeom).create();
	}

	template <typename T>
	const_iterator RCArray<T>::begin() const
	{
	}

	template <typename T>
	const_iterator RCArray<T>::end() const
	{
	}

	template <typename T>
	const_iterator RCArray<T>::cbegin() const
	{
	}

	template <typename T>
	const_iterator RCArray<T>::cend() const
	{
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

	
    } // namespace mpi
} // namespace CNORXZ

#endif
