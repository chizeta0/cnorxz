
#ifndef __cxz_func_cc_h__
#define __cxz_func_cc_h__

#include "func.h"

namespace CNORXZ
{

    template <typename... T>
    constexpr decltype(auto) ZeroF::operator()(const T&... as) const
    {
	return 0;
    }

    template <typename... T>
    constexpr decltype(auto) NoF::operator()(const T&... as) const
    {
	return;
    }

}

#endif
