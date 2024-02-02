// -*- C++ -*-
/**
   
   @file include/ranges/index_mul.h
   @brief Index multiplication

   Indices can be multiplied yielding index packs.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_index_mul_h__
#define __cxz_index_mul_h__

#include "base/base.h"
#include "index_base.h"

namespace CNORXZ
{
    struct MIndexSptrMul
    {
	template <class BlockT, class... Indices, class I, SizeT... Is>
	static decltype(auto) evalMX(const Sptr<GMIndex<BlockT,Indices...>>& a,
				     const Sptr<I>& b, Isq<Is...> is);

	template <class BlockT, class... Indices, class I, typename Meta, SizeT... Js>
	static decltype(auto) evalXM(const Sptr<I>& a, const Sptr<GMIndex<BlockT,Indices...>>& b,
				     Isq<Js...> js);

	template <class BlockT1, class... Indices1, class BlockT2, class... Indices2,
		  SizeT... Is, SizeT... Js>
	static decltype(auto) evalMM(const Sptr<GMIndex<BlockT1,Indices1...>>& a,
				     const Sptr<GMIndex<BlockT2,Indices2...>>& b,
				     Isq<Is...> is, Isq<Js...> js);
    };

    /** Combine two indices to a static index 2-pack.
	@param a First index.
	@param b Second index.
	@return Static index 2-pack
     */
    template <class I1, typename Meta1, class I2, typename Meta2>
    inline decltype(auto) operator*(const IndexInterface<I1,Meta1>& a,
				    const IndexInterface<I2,Meta2>& b);

    /** Extend static index pack on the l.h.s.
	@param a Index to be appended.
	@param b Index pack to be extended.
	@return Extended pack.
     */
    template <class I1, typename Meta1, class... Indices>
    inline decltype(auto) operator*(const IndexInterface<I1,Meta1>& a,
				    const SPack<Indices...>& b);

    /** Extend static index pack on the r.h.s.
	@param a Index pack to be extended.
	@param b Index to be appended.
	@return Extended pack.
     */
    template <class I2, typename Meta2, class... Indices>
    inline decltype(auto) operator*(const SPack<Indices...>& a,
				    const IndexInterface<I2,Meta2>& b);

    /** Combine two static index packs
	@param a First Index pack.
	@param b Second Index pack.
	@return New index pack. a is appended on the l.h.s. of b.
     */
    template <class... Indices1, class... Indices2>
    inline decltype(auto) operator*(const SPack<Indices1...>& a, const SPack<Indices2...>& b);

    /** Extend dynamic index pack on the l.h.s.
	@param a Index to be appended.
	@param b Index pack to be extended.
	@return Extended pack.
     */
    template <class I1, typename Meta1>
    inline decltype(auto) operator*(const IndexInterface<I1,Meta1>& a, const DPack& b);

    /** Extend dynamic index pack on the r.h.s.
	@param a Index pack to be extended.
	@param b Index to be appended.
	@return Extended pack.
     */
    template <class I2, typename Meta2>
    inline decltype(auto) operator*(const DPack& a, const IndexInterface<I2,Meta2>& b);

    /** Combine two dynamic index packs
	@param a First Index pack.
	@param b Second Index pack.
	@return New index pack. a is appended on the l.h.s. of b.
     */
    inline decltype(auto) operator*(const DPack& a, const DPack& b);

    /** Combine two index pointers to an index 2-pack.
	YIndices and DIndices will be combined into a DPack,
	otherwise a SPack is returned.
	@param a First index.
	@param b Second index.
	@return Index 2-pack.
     */
    template <class I1, class I2>
    decltype(auto) iptrMul(const Sptr<I1>& a, const Sptr<I2>& b);
}

#endif
