
#ifndef __cxz_index_mul_h__
#define __cxz_index_mul_h__

#include "base/base.h"
#include "index_base.h"

namespace CNORXZ
{
    struct MIndexMul
    {
	template <class BlockT, class... Indices, class I, typename Meta, SizeT... Is>
	static constexpr decltype(auto) evalMX(const GMIndex<BlockT,Indices...>& a,
					       const IndexInterface<I,Meta>& b,
					       Isq<Is...> is);

	template <class BlockT, class... Indices, class I, typename Meta, SizeT... Js>
	static constexpr decltype(auto) evalXM(const IndexInterface<I,Meta>& a,
					       const GMIndex<BlockT,Indices...>& b,
					       Isq<Js...> js);

	template <class BlockT1, class... Indices1, class BlockT2, class... Indices2,
		  SizeT... Is, SizeT... Js>
	static constexpr decltype(auto) evalMM(const GMIndex<BlockT1,Indices1...>& a,
					       const GMIndex<BlockT2,Indices2...>& b,
					       Isq<Is...> is, Isq<Js...> js);
    };

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

    template <class I1, typename Meta1, class I2, typename Meta2>
    constexpr decltype(auto) operator*(const IndexInterface<I1,Meta1>& a,
				       const IndexInterface<I2,Meta2>& b);

    template <class I1, class I2>
    decltype(auto) iptrMul(const Sptr<I1>& a, const Sptr<I2>& b);
}

#endif
