// -*- C++ -*-
/**
   
   @file opt/mpi/include/rrange.cc.h
   @brief RRange and RIndex declaration.
   
   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_rrange_cc_h__
#define __cxz_mpi_rrange_cc_h__

#include "rrange.h"

namespace CNORXZ
{
    namespace mpi
    {

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>::RIndex(const RIndex& in) :
	    mRange(in.mRange),
	    mI(std::make_shared<Index>(mRange->local())),
	    mK(std::make_shared<YIndex>(mRange->geom()))
	{
	    *this = in.lex();
	}

	template <class IndexI, class IndexK>
	RIndex& RIndex<IndexI,IndexK>::operator=(const RIndex& in)
	{
	    mRange = in.mRange;
	    mI = std::make_shared<Index>(mRange->local());
	    mK = std::make_shared<YIndex>(mRange->geom());
	    *this = in.lex();
	    return *this;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>::RIndex(const RangePtr& global, SizeT lexpos = 0) :
	    mRange(rangeCast<RangeType>(global)),
	    mI(std::make_shared<Index>(mRange->local())),
	    mK(std::make_shared<YIndex>(mRange->geom()))
	{
	    *this = lexpos;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>::RIndex(const Sptr<Index>& local) :
	{
	    //!!!
	}
	    
	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::operator=(SizeT pos)
	{
	    // pos is the lexicographic position of the global range.
	    // Hence, have to consider the rank geometry.
	    if constexpr(has_static_sub<IndexI>::value or has_static_sub<IndexK>::value){
		
	    }
	    else {

	    }
	    return *this;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::operator++()
	{
	    
	    return *this;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::operator--()
	{

	    return *this;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK> RIndex<IndexI,IndexK>::operator+(Int n) const
	{

	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK> RIndex<IndexI,IndexK>::operator-(Int n) const
	{

	}

	template <class IndexI, class IndexK>
	SizeT RIndex<IndexI,IndexK>::operator-(const RIndex& i) const
	{

	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::operator+=(Int n)
	{

	    return *this;
	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::operator-=(Int n)
	{

	    return *this;
	}

	template <class IndexI, class IndexK>
	SizeT RIndex<IndexI,IndexK>::lex() const
	{

	}

	template <class IndexI, class IndexK>
	constexpr RIndex<IndexI,IndexK>::decltype(auto) pmax() const
	{

	}
	    

	template <class IndexI, class IndexK>
	constexpr RIndex<IndexI,IndexK>::decltype(auto) lmax() const
	{

	}

	template <class IndexI, class IndexK>
	IndexId<0> RIndex<IndexI,IndexK>::id() const
	{

	}

	template <class IndexI, class IndexK>
	MetaType RIndex<IndexI,IndexK>::operator*() const
	{

	}
	
	template <class IndexI, class IndexK>
	constexpr SizeT RIndex<IndexI,IndexK>::dim() const
	{

	}

	template <class IndexI, class IndexK>
	Sptr<RangeType> RIndex<IndexI,IndexK>::range() const
	{

	}

	template <class IndexI, class IndexK>
	template <SizeT I>
	decltype(auto) RIndex<IndexI,IndexK>::stepSize(const IndexId<I>& id) const
	{

	}

	template <class IndexI, class IndexK>
	String RIndex<IndexI,IndexK>::stringMeta() const
	{

	}

	template <class IndexI, class IndexK>
	MetaType RIndex<IndexI,IndexK>::meta() const
	{

	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::at(const MetaType& metaPos)
	{

	}

	template <class IndexI, class IndexK>
	RangePtr RIndex<IndexI,IndexK>::prange(const RIndex<IndexI,IndexK>& last) const
	{

	}

	template <class IndexI, class IndexK>
	auto RIndex<IndexI,IndexK>::deepFormat() const
	{

	}

	template <class IndexI, class IndexK>
	auto RIndex<IndexI,IndexK>::deepMax() const
	{

	}

	template <class IndexI, class IndexK>
	RIndex<IndexI,IndexK>& RIndex<IndexI,IndexK>::reformat(const Vector<SizeT>& f, const Vector<SizeT>& s)
	{

	}

	template <class IndexI, class IndexK>
	template <class Xpr, class F>
	constexpr decltype(auto) RIndex<IndexI,IndexK>::ifor(const Xpr& xpr, F&& f) const
	{

	}

	template <class IndexI, class IndexK>
	bool RIndex<IndexI,IndexK>::formatIsTrivial() const
	{

	}

	template <class IndexI, class IndexK>
	decltype(auto) RIndex<IndexI,IndexK>::xpr(const Sptr<RIndex<IndexI,IndexK>>& _this) const
	{

	}

	template <class IndexI, class IndexK>
	int RIndex<IndexI,IndexK>::rank() const
	{

	}

	template <class IndexI, class IndexK>
	Sptr<Index> RIndex<IndexI,IndexK>::local() const
	{

	}
	
	
    } // namespace mpi
} // namespace CNORXZ

#endif
