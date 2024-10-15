
#ifndef __cxz_mpi_racc_xpr_cc_h__
#define __cxz_mpi_racc_xpr_cc_h__

#include "racc_xpr.h"

namespace CNOXRZ
{
    namespace mpi
    {
	template <SizeT L, class Xpr, class F = NoF>
	constexpr RankAccXpr<L,Xpr,F>::RankAccXpr(SizeT size, const IndexId<L>& id,
						  const Xpr& xpr, F&& f) :

	{}

	template <SizeT L, class Xpr, class F = NoF>
	template <class PosT>
	inline decltype(auto) RankAccXpr<L,Xpr,F>::operator()(const PosT& last) const
	{

	}

	template <SizeT L, class Xpr, class F = NoF>
	inline decltype(auto) RankAccXpr<L,Xpr,F>::operator()() const
	{

	}

	template <SizeT L, class Xpr, class F = NoF>
	template <SizeT I>
	inline decltype(auto) RankAccXpr<L,Xpr,F>::rootSteps(const IndexId<I>& id) const
	{

	}

	template <SizeT L, class Xpr, class F = NoF>
	constexpr decltype(auto) rankaccxpr(SizeT size, const IndexId<L>& id,
					    const Xpr& xpr, F&& f)
	{
	    return RankAccXpr<L,Xpr,F>(size, id, xpr, std::forward<F>(f));
	}
    }
}

#endif
