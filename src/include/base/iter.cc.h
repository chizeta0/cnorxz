// -*- C++ -*-
/**
   
   @file include/base/iter.cc.h
   @brief Static for-loops

   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_iter_cc_h__
#define __cxz_iter_cc_h__

#include "iter.h"
#include "xpr/func.h"

namespace CNORXZ
{
    /** @cond 0 */
    template <class G, class F, SizeT... Is>
    constexpr decltype(auto) iteri(const G& g, const F& f, Isq<Is...> is)
    {
	if constexpr(std::is_same<F,NoF>::value){
	    ( g(CSizeT<Is>{}), ... );
	}
	else {
	    return f( g(CSizeT<Is>{}) ... );
	}
    }
    /** @endcond */

    template <SizeT B, SizeT E, class G, class F>
    constexpr decltype(auto) iter(const G& g, const F& f)
    {
	return iteri(g, f, Isqr<B,E>{});
    }

    /** @cond 0 */
    template <SizeT E, SizeT I, class G, class F, class C, typename... Args>
    constexpr decltype(auto) iterIfi(const G& g, const F& f, const C& c, const Args&... args)
    {
	if constexpr(I >= E){
	    if constexpr(std::is_same<F,NoF>::value){
		return;
	    }
	    else {
		return f(args...);
	    }
	}
	else {
	    if constexpr(c(CSizeT<I>{})){
		return iterIfi<E,I+1>(g, f, c, args..., g(CSizeT<I>{}));
	    }
	    else {
		return iterIfi<E,I+1>(g, f, c, args...);
	    }
	}
    }
    /** @endcond */

    template <SizeT B, SizeT E, class G, class F, class C>
    constexpr decltype(auto) iterIf(const G& g, const F& f, const C& c)
    {
	return iterIfi<E,B>(g, f, c);
    }
}

#endif
