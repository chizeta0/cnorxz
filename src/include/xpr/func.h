// -*- C++ -*-
/**
   
   @file include/xpr/func.h
   @brief Special function objects declarations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

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
