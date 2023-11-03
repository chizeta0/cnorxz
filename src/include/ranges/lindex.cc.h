// -*- C++ -*-
/**
   
   @file include/ranges/lindex.cc.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_lindex_cc_h__
#define __cxz_lindex_cc_h__

#include "lindex.h"

namespace CNORXZ
{
    template <class Index, SizeT L>
    LIndex<Index,L>::LIndex(const Sptr<Index>& i) :
	Index(*i),
	mI(i)
    {}

    template <class Index, SizeT L>
    IndexId<L> LIndex<Index,L>::id() const
    {
	return IndexId<L>(mI->ptrId());
    }

    template <class Index, SizeT L>
    template <SizeT I>
    decltype(auto) LIndex<Index,L>::stepSize(const IndexId<I>& id) const
    {
	if constexpr(L == 0 and I == 0){
	    return UPos(mI->id() == id ? 1 : 0);
	}
	else {
	    if constexpr(L == I) {
		return SPos<1>();
	    }
	    else {
		return SPos<0>();
	    }
	}
    }

    template <class Index, SizeT L>
    template <class Xpr, class F>
    decltype(auto) LIndex<Index,L>::ifor(const Xpr& xpr, F&& f) const
    {
	return For<L,Xpr,F>(this->pmax().val(), this->id(), xpr, std::forward<F>(f));
    }
    
    template <class Index, SizeT L, class I1>
    decltype(auto) operator*(const Sptr<LIndex<Index,L>>& a, const Sptr<I1>& b)
    {
	return iptrMul(a, b);
    }

    template <SizeT L, class Index>
    decltype(auto) lindexPtr(const Sptr<Index>& i)
    {
	return std::make_shared<LIndex<Index,L>>( i );
    }

    template <class Index, SizeT L>
    decltype(auto) lindexPtr(const Sptr<Index>& i, CSizeT<L> l)
    {
	return lindexPtr<l>( i );
    }
}

#endif
