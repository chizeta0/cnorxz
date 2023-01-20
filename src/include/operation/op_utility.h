
#ifndef __cxz_op_utility_h__
#define __cxz_op_utility_h__

#include "base/base.h"

namespace CNORXZ
{
    template <SizeT I, class PosT>
    constexpr decltype(auto) pos_get(const PosT& pos);
    
    template <SizeT J, SizeT... Is>
    constexpr SizeT sum_index_sequence(std::index_sequence<Is...> is);

    template <class F, class PosT, class OpTuple, class OpSizes, SizeT... Is>
    inline auto pos_unpack_args_i(const F& f, const PosT& pos, const OpTuple& args,
				  OpSizes opsizes, std::index_sequence<Is...> is);
    
    template <class F, class PosT, class... Ops>
    inline auto pos_unpack_args(const F& f, const PosT& pos, const Tuple<Ops...>& args);
    
}

#endif
