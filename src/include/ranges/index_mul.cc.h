
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
	return MIndex<Indices...,I>( a.pack()[std::integral_constant<SizeT,Is>{}]...,
				     std::make_shared<I>(b.THIS()) );
    }
    
    template <class BlockT, class... Indices, class I, typename Meta, SizeT... Js>
    constexpr decltype(auto) MIndexMul::evalXM(const IndexInterface<I,Meta>& a,
					       const GMIndex<BlockT,Indices...>& b,
					       Isq<Js...> js)
    {
	static_assert(sizeof...(Js) == sizeof...(Indices), "inconsistent index sequence");
	return MIndex<Indices...,I>( std::make_shared<I>(a.THIS()),
				     b.pack()[std::integral_constant<SizeT,Js>{}]... );
    }
    
    template <class BlockT1, class... Indices1, class BlockT2, class... Indices2,
	      SizeT... Is, SizeT... Js>
    constexpr decltype(auto) MIndexMul::evalMM(const GMIndex<BlockT1,Indices1...>& a,
					       const GMIndex<BlockT2,Indices2...>& b,
					       Isq<Is...> is, Isq<Js...> js)
    {
	static_assert(sizeof...(Is) == sizeof...(Indices1), "inconsistent index sequence");
	static_assert(sizeof...(Js) == sizeof...(Indices2), "inconsistent index sequence");
	return MIndex<Indices1...,Indices2...>( a.pack()[std::integral_constant<SizeT,Is>{}]...,
						b.pack()[std::integral_constant<SizeT,Js>{}]... );
    }

    /*********************
     *   MIndexSptrMul   *
     *********************/

    template <class BlockT, class... Indices, class I, SizeT... Is>
    decltype(auto) MIndexSptrMul::evalMX(const Sptr<GMIndex<BlockT,Indices...>>& a,
					 const Sptr<I>& b, Isq<Is...> is)
    {
	static_assert(sizeof...(Is) == sizeof...(Indices), "inconsistent index sequence");
	return std::make_shared<MIndex<Indices...,I>>( a->pack()[std::integral_constant<SizeT,Is>{}]..., b );
    }
    
    template <class BlockT, class... Indices, class I, typename Meta, SizeT... Js>
    decltype(auto) MIndexSptrMul::evalXM(const Sptr<I>& a,
					 const Sptr<GMIndex<BlockT,Indices...>>& b,
					 Isq<Js...> js)
    {
	static_assert(sizeof...(Js) == sizeof...(Indices), "inconsistent index sequence");
	return std::make_shared<MIndex<I,Indices...>>( a, b->pack()[std::integral_constant<SizeT,Js>{}]... );
    }
    
    template <class BlockT1, class... Indices1, class BlockT2, class... Indices2,
	      SizeT... Is, SizeT... Js>
    decltype(auto) MIndexSptrMul::evalMM(const Sptr<GMIndex<BlockT1,Indices1...>>& a,
					 const Sptr<GMIndex<BlockT2,Indices2...>>& b,
					 Isq<Is...> is, Isq<Js...> js)
    {
	static_assert(sizeof...(Is) == sizeof...(Indices1), "inconsistent index sequence");
	static_assert(sizeof...(Js) == sizeof...(Indices2), "inconsistent index sequence");
	return MIndex<Indices1...,Indices2...>( a->pack()[std::integral_constant<SizeT,Is>{}]...,
						b->pack()[std::integral_constant<SizeT,Js>{}]... );
    }

    
    /*****************
     *   operator*   *
     *****************/
    
    template <class I1, typename Meta1, class I2, typename Meta2>
    constexpr decltype(auto) operator*(const IndexInterface<I1,Meta1>& a,
				       const IndexInterface<I2,Meta2>& b)
    {
	if constexpr(std::is_same<I1,DIndex>::value){
	    if constexpr(std::is_same<I2,DIndex>::value){
		return YIndex({ a.THIS().xptr(), b.THIS().xptr() });
	    }
	    else if constexpr(std::is_same<I2,YIndex>::value){
		auto p = b.THIS().pack();
		auto n = p.lmul( a.THIS().xptr() );
		return YIndex(n.all());
	    }
	}
	else if constexpr(std::is_same<I1,YIndex>::value){
	    if constexpr(std::is_same<I2,DIndex>::value){
		auto p = a.THIS().pack();
		auto n = p.rmul( b.THIS().xptr() );
		return YIndex(n.all());
	    }
	    else if constexpr(std::is_same<I2,YIndex>::value){
		auto ap = a.THIS().pack().all();
		const auto& bp = b.THIS().pack();
		ap.insert(ap.end(), bp.all().begin(), bp.all().end());
		return YIndex(ap);
	    }
	}
	else {
	    constexpr SizeT I1D = index_dim<I1>::value;
	    constexpr SizeT I2D = index_dim<I2>::value;
	    if constexpr(I1D == 1){
		if constexpr(I2D == 1){
		    return MIndex<I1,I2>(a.THIS(),b.THIS());
		}
		else {
		    return MIndexMul::evalXM(a, b.THIS(), std::make_index_sequence<I2D>{});
		}
	    }
	    else {
		if constexpr(I2D == 1){
		    return MIndexMul::evalMX(a.THIS(), b, std::make_index_sequence<I1D>{});
		}
		else {
		    return MIndexMul::evalMM(a.THIS(), b.THIS(),
					     std::make_index_sequence<I1D>{},
					     std::make_index_sequence<I2D>{});
		}
	    }
	}
    }

    /***************
     *   iptrMul   *
     ***************/

    template <class I1, class I2>
    decltype(auto) iptrMul(const Sptr<I1>& a, const Sptr<I2>& b)
    {
	if constexpr(std::is_same<I1,DIndex>::value){
	    if constexpr(std::is_same<I2,DIndex>::value){
		return std::make_shared<YIndex>({ a->xptr(), b->xptr() });
	    }
	    else if constexpr(std::is_same<I2,YIndex>::value){
		auto p = b->pack();
		p.insert(0, a->xptr());
		return std::make_shared<YIndex>(p);
	    }
	}
	else if constexpr(std::is_same<I1,YIndex>::value){
	    if constexpr(std::is_same<I2,DIndex>::value){
		auto p = a->pack();
		p.push_back(b->xptr());
		return std::make_shared<YIndex>(p);
	    }
	    else if constexpr(std::is_same<I2,YIndex>::value){
		auto p = a->pack();
		p.insert(p.end(), b->pack().begin(), b->pack().end());
		return std::make_shared<YIndex>(p);
	    }
	}
	else {
	    constexpr SizeT I1D = index_dim<I1>::value;
	    constexpr SizeT I2D = index_dim<I2>::value;
	    if constexpr(I1D == 1){
		if constexpr(index_dim<I2>::value == 1){
		    return std::make_shared<MIndex<I1,I2>>(a, b);
		}
		else {
		    return MIndexSptrMul::evalXM(a, b, std::make_index_sequence<I1D>{});
		}
	    }
	    else {
		if constexpr(index_dim<I2>::value == 1){
		    return MIndexSptrMul::evalMX(a, b, std::make_index_sequence<I2D>{});
		}
		else {
		    return MIndexSptrMul::evalMM(a, b, std::make_index_sequence<I1D>{},
						 std::make_index_sequence<I2D>{});
		}
	    }
	}
    }

}

#endif
