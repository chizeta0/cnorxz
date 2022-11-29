
#ifndef __cxz_index_mul_cc_h__
#define __cxz_index_mul_cc_h__

#include "index_mul.h"

namespace CNORXZ
{
    /*****************
     *   MIndexMul   *
     *****************/
    
    template <class BlockT, class... Indices, class I, typename Meta, SizeT... Is>
    constexpr decltype(auto) MIndexMul::evalMX(const GMIndex<BlockT,Indices...>& a,
					       const IndexInterface<I,Meta>& b,
					       Isq<Is...> is)
    {
	static_assert(sizeof...(Is) == sizeof...(Indices), "inconsistent index sequence");
	return MIndex<Indices...,I>( std::get<Is>(a.pack())..., b.THIS() );
    }
    
    template <class BlockT, class... Indices, class I, typename Meta, SizeT... Js>
    constexpr decltype(auto) MIndexMul::evalXM(const IndexInterface<I,Meta>& a,
					       const GMIndex<BlockT,Indices...>& b,
					       Isq<Js...> js)
    {
	static_assert(sizeof...(Js) == sizeof...(Indices), "inconsistent index sequence");
	return MIndex<Indices...,I>( a.THIS(), std::get<Js>(b.pack())... );
    }
    
    template <class BlockT1, class... Indices1, class BlockT2, class... Indices2,
	      SizeT... Is, SizeT... Js>
    constexpr decltype(auto) MIndexMul::evalMM(const GMIndex<BlockT1,Indices1...>& a,
					       const GMIndex<BlockT2,Indices2...>& b,
					       Isq<Is...> is, Isq<Js...> js)
    {
	static_assert(sizeof...(Is) == sizeof...(Indices1), "inconsistent index sequence");
	static_assert(sizeof...(Js) == sizeof...(Indices2), "inconsistent index sequence");
	return MIndex<Indices1...,Indices2...>( std::get<Is>(a.pack())...,
						std::get<Js>(b.pack())... );
    }

    /*********************
     *   MIndexSptrMul   *
     *********************/

    template <class BlockT, class... Indices, class I, SizeT... Is>
    decltype(auto) MIndexSptrMul::evalMX(const Sptr<GMIndex<BlockT,Indices...>>& a,
					 const Sptr<I>& b, Isq<Is...> is)
    {
	static_assert(sizeof...(Is) == sizeof...(Indices), "inconsistent index sequence");
	return std::make_shared<MIndex<Indices...,I>>( std::get<Is>(a->pack())..., b );
    }
    
    template <class BlockT, class... Indices, class I, typename Meta, SizeT... Js>
    decltype(auto) MIndexSptrMul::evalXM(const Sptr<I>& a,
					 const Sptr<GMIndex<BlockT,Indices...>>& b,
					 Isq<Js...> js)
    {
	static_assert(sizeof...(Js) == sizeof...(Indices), "inconsistent index sequence");
	return std::make_shared<MIndex<I,Indices...>>( a, std::get<Js>(b->pack())... );
    }
    
    template <class BlockT1, class... Indices1, class BlockT2, class... Indices2,
	      SizeT... Is, SizeT... Js>
    decltype(auto) MIndexSptrMul::evalMM(const Sptr<GMIndex<BlockT1,Indices1...>>& a,
					 const Sptr<GMIndex<BlockT2,Indices2...>>& b,
					 Isq<Is...> is, Isq<Js...> js)
    {
	static_assert(sizeof...(Is) == sizeof...(Indices1), "inconsistent index sequence");
	static_assert(sizeof...(Js) == sizeof...(Indices2), "inconsistent index sequence");
	return MIndex<Indices1...,Indices2...>( std::get<Is>(a->pack())...,
						std::get<Js>(b->pack())... );
    }

    
    /*****************
     *   operator*   *
     *****************/
    
    template <class I1, typename Meta1, class I2, typename Meta2>
    constexpr decltype(auto) operator*(const IndexInterface<I1,Meta1>& a,
				       const IndexInterface<I2,Meta2>& b)
    {
	// special operations for DIndex / YIndex
	if constexpr(index_dim<I1>::value == 1){
	    if constexpr(index_dim<I2>::value == 1){
		return MIndex<I1,I2>(a.THIS(),b.THIS());
	    }
	    else {
		return MIndexMul::evalXM(a, b.THIS());
	    }
	}
	else {
	    if constexpr(index_dim<I2>::value == 1){
		return MIndexMul::evalMX(a.THIS(), b);
	    }
	    else {
		return MIndexMul::evalMM(a.THIS(), b.THIS());
	    }
	}
    }

    /***************
     *   iptrMul   *
     ***************/

    template <class I1, class I2>
    decltype(auto) iptrMul(const Sptr<I1>& a, const Sptr<I2>& b)
    {
	// special operations for DIndex / YIndex
	if constexpr(index_dim<I1>::value == 1){
	    if constexpr(index_dim<I2>::value == 1){
		return std::make_shared<MIndex<I1,I2>>(a, b);
	    }
	    else {
		return MIndexSptrMul::evalXM(a, b);
	    }
	}
	else {
	    if constexpr(index_dim<I2>::value == 1){
		return MIndexSptrMul::evalMX(a, b);
	    }
	    else {
		return MIndexSptrMul::evalMM(a, b);
	    }
	}
    }

}

#endif
