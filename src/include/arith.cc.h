
#include "arith.h"

namespace CNORXZ
{

    template <class F>
    template <class... Ops>
    auto StaticFunctionBase<F>::mk(const Ops&... ops)
    {
        return Operation<typename F::value_type,F,Ops...>(ops...);
    }

    template <class F>
    template <size_t N, class Tuple, typename... As>
    inline auto StaticFunctionBase<F>::xapply(const Tuple& tp, As... as)
    {
        if constexpr(N > 0){
            return xapply<N-1>(tp, std::get<N>(tp), as...);
        }
        else {
            return F::apply(std::get<0>(tp), as...);
        }
    }

    template <class F>
    template <typename... As>
    inline auto StaticFunctionBase<F>::apply(const std::tuple<As...>& arg)
    {
        return xapply<sizeof...(As)-1>(arg);
        //return ArgPack<sizeof...(As)-1>::template mk<F,std::tuple<As...> >(arg);
    }

    
}
