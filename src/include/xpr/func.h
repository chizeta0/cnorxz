
#ifndef __cxz_func_h__
#define __cxz_func_h__

#include "base/base.h"

namespace CNORXZ
{
    class ZeroF
    {
    public:
	template <typename... T>
	constexpr decltype(auto) operator()(const T&... as) const;
    };

    class NoF
    {
    public:
	template <typename... T>
	constexpr decltype(auto) operator()(const T&... as) const;
    };

}

#endif
