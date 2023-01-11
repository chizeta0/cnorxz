
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
