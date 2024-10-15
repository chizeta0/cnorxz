
// rank access expression, fix rank position to current rank

#ifndef __cxz_racc_xpr_h__
#define __cxz_racc_xpr_h__

#include "mpi_base.h"

namespace CNORXZ
{
    namespace mpi
    {

	template <SizeT L, class Xpr, class F = NoF>
	class AccXpr : public XprInterface<AccXpr<L,Xpr,F>>
	{
	public:
	    DEFAULT_MEMBERS(AccXpr);

	    constexpr AccXpr(SizeT n, const IndexId<L>& id, const Xpr& xpr, F&& f);

	    template <class PosT>
	    inline decltype(auto) operator()(const PosT& last) const;

	    inline decltype(auto) operator()() const;

	    template <SizeT I>
	    inline decltype(auto) rootSteps(const IndexId<I>& id) const;
	    
	private:
	    SizeT mN = 0;
	    IndexId<L> mId;
	    Xpr mXpr;
	    typedef decltype(mXpr.rootSteps(mId)) XPosT;
	    XPosT mExt;
	    F mF;
	};

	template <SizeT L, class Xpr, class F = NoF>
	constexpr decltype(auto) accxpr(SizeT n, const IndexId<L>& id, const Xpr& xpr, F&& f);
    }
}

#endif
