// -*- C++ -*-
/**
   
   @file include/ranges/index_mul.cc.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_index_mul_cc_h__
#define __cxz_index_mul_cc_h__

#include "index_mul.h"

namespace CNORXZ
{

    /*********************
     *   MIndexSptrMul   *
     *********************/

    template <class BlockT, class... Indices, class I, SizeT... Is>
    decltype(auto) MIndexSptrMul::evalMX(const Sptr<GMIndex<BlockT,Indices...>>& a,
					 const Sptr<I>& b, Isq<Is...> is)
    {
	static_assert(sizeof...(Is) == sizeof...(Indices), "inconsistent index sequence");
	return std::make_shared<MIndex<Indices...,I>>( a->pack()[CSizeT<Is>{}]..., b );
    }
    
    template <class BlockT, class... Indices, class I, typename Meta, SizeT... Js>
    decltype(auto) MIndexSptrMul::evalXM(const Sptr<I>& a,
					 const Sptr<GMIndex<BlockT,Indices...>>& b,
					 Isq<Js...> js)
    {
	static_assert(sizeof...(Js) == sizeof...(Indices), "inconsistent index sequence");
	return std::make_shared<MIndex<I,Indices...>>( a, b->pack()[CSizeT<Js>{}]... );
    }
    
    template <class BlockT1, class... Indices1, class BlockT2, class... Indices2,
	      SizeT... Is, SizeT... Js>
    decltype(auto) MIndexSptrMul::evalMM(const Sptr<GMIndex<BlockT1,Indices1...>>& a,
					 const Sptr<GMIndex<BlockT2,Indices2...>>& b,
					 Isq<Is...> is, Isq<Js...> js)
    {
	static_assert(sizeof...(Is) == sizeof...(Indices1), "inconsistent index sequence");
	static_assert(sizeof...(Js) == sizeof...(Indices2), "inconsistent index sequence");
	return MIndex<Indices1...,Indices2...>( a->pack()[CSizeT<Is>{}]...,
						b->pack()[CSizeT<Js>{}]... );
    }

    
    /*****************
     *   operator*   *
     *****************/

    template <class I1, typename Meta1, class I2, typename Meta2>
    inline decltype(auto) operator*(const IndexInterface<I1,Meta1>& a,
				    const IndexInterface<I2,Meta2>& b)
    {
	if constexpr(std::is_same<I1,YIndex>::value or std::is_same<I2,YIndex>::value or
		     std::is_same<I1,DIndex>::value or std::is_same<I2,DIndex>::value) {
	    return dpack(a.THIS(), b.THIS());
	}
	else {
	    return spack(a.THIS(), b.THIS());
	}
    }

    template <class I1, typename Meta1, class... Indices>
    inline decltype(auto) operator*(const IndexInterface<I1,Meta1>& a,
				    const SPack<Indices...>& b)
    {
	return b.lmul(std::make_shared<I1>(a.THIS()));
    }

    template <class I2, typename Meta2, class... Indices>
    inline decltype(auto) operator*(const SPack<Indices...>& a,
				    const IndexInterface<I2,Meta2>& b)
    {
	return a.rmul(std::make_shared<I2>(b.THIS()));
    }

    template <class Index, class... Indices>
    inline decltype(auto) operator*(const Sptr<Index>& a,
				    const SPack<Indices...>& b)
    {
	static_assert(is_index<Index>::value, "got non-index type");
	return b.lmul(a);
    }

    template <class Index, class... Indices>
    inline decltype(auto) operator*(const SPack<Indices...>& a,
				    const Sptr<Index>& b)
    {
	static_assert(is_index<Index>::value, "got non-index type");
	return a.rmul(b);
    }

    template <class... Indices1, class... Indices2>
    inline decltype(auto) operator*(const SPack<Indices1...>& a, const SPack<Indices2...>& b)
    {
	return a.mul(b);
    }

    template <class I1, typename Meta1>
    inline decltype(auto) operator*(const IndexInterface<I1,Meta1>& a, const DPack& b)
    {
	return b.lmul(std::make_shared<I1>(a.THIS()));
    }

    template <class I2, typename Meta2>
    inline decltype(auto) operator*(const DPack& a, const IndexInterface<I2,Meta2>& b)
    {
	return a.rmul(std::make_shared<I2>(b.THIS()));
    }

    inline decltype(auto) operator*(const DPack& a, const DPack& b)
    {
	return a.mul(b);
    }

    /***************
     *   iptrMul   *
     ***************/

    template <class I1, class I2>
    decltype(auto) iptrMul(const Sptr<I1>& a, const Sptr<I2>& b)
    {
	static_assert(is_index<I1>::value and is_index<I2>::value, "got non-index type");
	if constexpr(std::is_same<I1,YIndex>::value or std::is_same<I2,YIndex>::value or
		     std::is_same<I1,DIndex>::value or std::is_same<I2,DIndex>::value) {
	    return dpackp(a, b);
	}
	else {
	    return spackp(a, b);
	}
    }

}

#endif
