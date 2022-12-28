
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

    template <SizeT E, SizeT I, class G, class F, class C, typename... Args>
    constexpr decltype(auto) iterIfi(const G& g, const F& f, const C& c, const Args&... args);

    template <SizeT B, SizeT E, class G, class F, class C>
    constexpr decltype(auto) iterIf(const G& g, const F& f, const C& c);
}

#endif
