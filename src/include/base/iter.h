// -*- C++ -*-
/**
   
   @file include/base/iter.h
   @brief Static for-loops

   Declaration of function templates that can be used to implement a static for-loop
   
   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_iter_h__
#define __cxz_iter_h__

#include <cstdlib>
#include "types.h"

namespace CNORXZ
{
    /** @cond 0 */
    template <class G, class F, SizeT... Is>
    constexpr decltype(auto) iteri(const G& g, const F& f, Isq<Is...> is);
    /** @endcond */
    
    /** static for loop
	@tparam B begin index
	@tparam E end index
	@tparam G type of expression to executed for each element
	@tparam F type of accumulating expression collecting result for all elements
	@param g expression to executed for each element
	@param f accumulating expression collecting result for all elements
     */
    template <SizeT B, SizeT E, class G, class F>
    constexpr decltype(auto) iter(const G& g, const F& f);

    /** @cond 0 */
    template <SizeT E, SizeT I, class G, class F, class C, typename... Args>
    constexpr decltype(auto) iterIfi(const G& g, const F& f, const C& c, const Args&... args);
    /** @endcond */

    /** static conditional for loop
	@tparam B begin index
	@tparam E end index
	@tparam G type of expression to executed for each element
	@tparam F type of accumulating expression collecting result for all elements
	@tparam C type of condition expression
	@param g expression to executed for each element
	@param f accumulating expression collecting result for all elements
	@param c condition expression
     */
    template <SizeT B, SizeT E, class G, class F, class C>
    constexpr decltype(auto) iterIf(const G& g, const F& f, const C& c);
}

#endif
