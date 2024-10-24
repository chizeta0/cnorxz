
#ifndef __cxz_acc_xpr_cc_h__
#define __cxz_acc_xpr_cc_h__

#include "acc_xpr.h"

namespace CNORXZ
{
    template <SizeT L, class Xpr, class F>
    constexpr AccXpr<L,Xpr,F>::AccXpr(SizeT n, const IndexId<L>& id,
				      const Xpr& xpr, F&& f) :
	mN(n),
	mId(id),
	mXpr(xpr),
	mExt(mXpr.rootSteps(mId)),
	mF(std::forward<F>(f))
    {}

    template <SizeT L, class Xpr, class F>
    template <class PosT>
    inline decltype(auto) AccXpr<L,Xpr,F>::operator()(const PosT& last) const
    {
	if constexpr(std::is_same<typename std::remove_reference<F>::type,NoF>::value){
	    const auto pos = last + mExt( UPos(mN) );
	    mXpr(pos);
	    return None {};
	}
	else {
	    typedef typename
		std::remove_reference<decltype(mXpr(last + mExt( UPos(0) )))>::type OutT;
	    auto o = OutT();
	    const auto pos = last + mExt( UPos(mN) );
	    mF(o, mXpr(pos));
	    return o;
	}
    }

    template <SizeT L, class Xpr, class F>
    inline decltype(auto) AccXpr<L,Xpr,F>::operator()() const
    {
	if constexpr(std::is_same<typename std::remove_reference<F>::type,NoF>::value){
	    const auto pos = mExt( UPos(mN) );
	    mXpr(pos);
	    return None {};
	}
	else {
	    typedef typename
		std::remove_reference<decltype(mXpr( mExt( UPos(0) )))>::type OutT;
	    auto o = OutT();
	    const auto pos = mExt( UPos(mN) );
	    mF(o, mXpr(pos));
	    return o;
	}
    }

    template <SizeT L, class Xpr, class F>
    template <SizeT I>
    inline decltype(auto) AccXpr<L,Xpr,F>::rootSteps(const IndexId<I>& id) const
    {
	return mXpr.rootSteps(id);
    }

    template <SizeT L, class Xpr, class F>
    constexpr decltype(auto) accxpr(SizeT n, const IndexId<L>& id, const Xpr& xpr, F&& f)
    {
	return AccXpr<L,Xpr,F>(n, id, xpr, std::forward<F>(f));
    }
}

#endif