
#ifndef __cxz_op_expr_cc_h__
#define __cxz_op_expr_cc_h__

#include "op_expr.h"
#include "op_utility.h"

namespace CNORXZ
{
    template <class F, class... Ops>
    constexpr OpXpr<F,Ops...>::OpXpr(F&& f, Ops&&... ops) :
	mF(std::forward<F>(f)),
	mOps(ops...)
    {
	static_assert((is_operation<Ops>::value and ...), "got non-op type");
    }
	
    template <class F, class... Ops>
    template <class PosT1, class PosT2>
    inline SizeT OpXpr<F,Ops...>::operator()(const PosT& last) const
    {
	pos_unpack_args(mF,last,mOps); // utility function (to be implemented)
	// depending on whether Ops[N] is static or not call statically or dynamically .next()
	return 0;
    }

    template <class F, class... Ops>
    inline SizeT OpXpr<F,Ops...>::operator()() const
    {
	return 0;
    }

    template <class F, class... Ops>
    template <SizeT I>
    constexpr decltype(auto) OpXpr<F,Ops...>::rootSteps(const IndexId<I>& id) const
    {
	return rootStepsI(id, std::make_index_sequence<sizeof...(Ops)>{});
    }

    template <class F, class... Ops>
    template <SizeT I, SizeT... Js>
    constexpr decltype(auto) OpXpr<F,Ops...>::rootStepsI(const IndexId<I>& id, std::index_sequence<Js...> is) const
    {
	return (std::get<Js>(mOps).rootSteps(id) << ...);
	// TODO: implement a << b which is a.extend(b)!!  
    }
}

#endif
