
#ifndef __cxz_iter_cc_h__
#define __cxz_iter_cc_h__

#include "iter.h"
#include "functional/fundamental.h"

namespace CNORXZ
{
    template <class G, class F, SizeT... Is>
    constexpr decltype(auto) iteri(const G& g, const F& f, Isq<Is...> is)
    {
	if constexpr(std::is_same<F,NoF>::value){
	    ( g(std::integral_constant<SizeT,Is>{}, args...), ... );
	}
	else {
	    return f( g(std::integral_constant<SizeT,Is>{}, args...) ... );
	}
    }

    template <SizeT B, SizeT E, class G, class F>
    constexpr decltype(auto) iter(const G& g, const F& f)
    {
	return iteri(g, f, Isqr<B,E>{});
    }
}

#endif
