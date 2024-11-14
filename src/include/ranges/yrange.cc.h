// -*- C++ -*-
/**
   
   @file include/ranges/yrange.cc.h
   @brief YRange and YIndex template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_yrange_cc_h__
#define __cxz_yrange_cc_h__

#include "yrange.h"
#include "index_mul.h"

namespace CNORXZ
{
    template <class Xpr>
    DXpr<None> YIndex::ifor(const Xpr& xpr, NoF&& f) const
    {
	return this->ifor( DXpr<None>(xpr), std::forward<NoF>(f) );
    }

    template <class I>
    decltype(auto) operator*(const Sptr<YIndex>& a, const Sptr<I>& b)
    {
	return iptrMul(a, b);
    }

}

#endif
