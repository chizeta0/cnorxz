
#ifndef __cxz_iter_h__
#define __cxz_iter_h__

#include <cstdlib>
#include "types.h"

namespace CNORXZ
{
    template <class G, class F, SizeT... Is>
    constexpr decltype(auto) iteri(const G& g, const F& f, Isq<Is...> is);

    template <SizeT B, SizeT E, class G, class F>
    constexpr decltype(auto) iter(const G& g, const F& f);
}

#endif
