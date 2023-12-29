// -*- C++ -*-
/**
   
   @file include/ranges/dindex.cc.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_dindex_cc_h__
#define __cxz_dindex_cc_h__

#include "dindex.h"

namespace CNORXZ
{
    template <class Index, typename Meta>
    DIndex::DIndex(const IndexInterface<Index,Meta>& i) :
	IndexInterface<DIndex,DType>(i.pos()),
	mI(std::make_shared<XIndex<Index,Meta>>(i))
    {}

    template <class Xpr, class F>
    decltype(auto) DIndex::ifor(const Xpr& xpr, F&& f) const
    {
	typedef typename std::remove_reference<decltype(Xpr()())>::type R;
	if constexpr(std::is_same<F,NoF>::value) {
	    if constexpr(std::is_same<R,None>::value or std::is_same<R,void>::value){
		return mI->ifor( DXpr<None>(xpr), std::forward<F>(f) );
	    }
	    else {
		return DXpr<R>( bufxpr([&](auto x){ return mI->ifor( DXpr<None>( assignxpr(x, xpr) ), NoF {} ); }) );
	    }
	}
	else {
	    return DXpr<R>( bufxpr([&](auto x){ return mI->ifor( DXpr<None>( assignxpr(x, xpr) ), NoF {} ); }) );
	}
    }

    template <class I>
    I indexAs(const DIndex& i)
    {
	static_assert(is_index<I>::value, "expected index type");
	auto ptr = std::dynamic_pointer_cast<XIndex<I,typename I::MetaType>>( i.xptr() );
	CXZ_ASSERT(ptr, "invalid cast");
	return ptr->get();
    }
}

#endif
