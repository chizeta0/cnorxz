// -*- C++ -*-
/**
   
   @file include/operation/op_utility.cc.h
   @brief Operation utilities template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_op_utility_cc_h__
#define __cxz_op_utility_cc_h__

#include "op_utility.h"
#include "xpr/pos_type.h"

namespace CNORXZ
{
    template <SizeT I, class PosT>
    constexpr decltype(auto) pos_get(const PosT& pos)
    {
	static_assert(I < static_pos_size<PosT>::value, "index out of range");
	if constexpr(I == 0){
	    return pos;
	}
	else {
	    return pos_get<I-1>(pos.next());
	}
    }

    template <SizeT J, SizeT I, SizeT... Is>
    constexpr SizeT sum_index_sequence(std::index_sequence<I,Is...> is)
    {
	static_assert(J < sizeof...(Is)+1, "index out of range");
	if constexpr(J == 0){
	    return 0;
	}
	else {
	    return sum_index_sequence<J-1>(std::index_sequence<Is...>{}) + I;
	}
    }

    template <class F, class PosT, class OpTuple, class OpSizes, SizeT... Is>
    inline auto pos_unpack_args_i(const F& f, const PosT& pos, const OpTuple& args,
				  OpSizes opsizes, std::index_sequence<Is...> is)
    {
	return f(std::get<Is>(args)(pos_get<sum_index_sequence<Is>(opsizes)>(pos))...);
    }

    template <class F, class PosT, class... Ops>
    inline auto pos_unpack_args(const F& f, const PosT& pos, const Tuple<Ops...>& args)
    {
	static_assert(is_pos_type<PosT>::value, "got non-pos-type");
	static_assert((is_xpr<Ops>::value and ...), "got non-operation type");
	typedef std::make_index_sequence<sizeof...(Ops)> Idxs;
	typedef std::index_sequence<op_size<Ops>::value...> OpSizes;
	return pos_unpack_args_i(f, pos, args, OpSizes{}, Idxs{});
    }

}

#endif
