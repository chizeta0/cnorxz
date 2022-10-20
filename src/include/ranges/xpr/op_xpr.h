
#ifndef __cxz_op_expr_h__
#define __cxz_op_expr_h__

#include "base/base.h"
#include "operation/operation_base.h"
#include "xpr_base.h"

namespace CNORXZ
{

    template <class F, class... Ops>
    class OpXpr
    {
    private:
	F mF;
	Tuple<Ops...> mOps;

	template <SizeT I, SizeT... Js>
	constexpr decltype(auto) rootStepsI(const IndexId<I>& id, std::index_sequence<Js...> is) const;
 	
    public:
	DEFAULT_MEMBERS(OpXpr);

	constexpr OpXpr(F&& f, Ops&&... ops);

	template <class PosT>
	inline SizeT operator()(const PosT& last) const;

	inline SizeT operator()() const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;
    };
}

#endif
