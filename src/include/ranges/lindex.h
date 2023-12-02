// -*- C++ -*-
/**
   
   @file include/ranges/lindex.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_lindex_h__
#define __cxz_lindex_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    // static label to enforce loop unrolling
    template <class Index, SizeT L>
    class LIndex : public Index
    {
    public:
	typedef typename Index::IB IB;
	typedef typename Index::RangeType RangeType;

	DEFAULT_MEMBERS(LIndex);
	LIndex(const Sptr<Index>& i);

	IndexId<L> id() const;
	
	template <SizeT I>
	decltype(auto) stepSize(const IndexId<I>& id) const;

	template <class Xpr, class F>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;

    private:
	Sptr<Index> mI;
    };

    template <class Index, SizeT L>
    struct is_index<LIndex<Index,L>>
    {
	static constexpr bool value = is_index<Index>::value; 
    };

    /** ***
	LIndex can be used as expression if this is case for its base type
	@see index_expression_exists
     */    
    template <class Index, SizeT L>
    struct index_expression_exists<LIndex<Index,L>>
    {
	static constexpr bool value = index_expression_exists<Index>::value;
    };

    template <class Index, SizeT L, class I1>
    decltype(auto) operator*(const Sptr<LIndex<Index,L>>& a, const Sptr<I1>& b);

    template <SizeT L, class Index>
    decltype(auto) lindexPtr(const Sptr<Index>& i);

    template <class Index, SizeT L>
    decltype(auto) lindexPtr(const Sptr<Index>& i, CSizeT<L> l);
}

#endif
