
#ifndef __cxz_for_cc_h__
#define __cxz_for_cc_h__

#include <omp.h>

#include "for.h"

namespace CNORXZ
{
    /***********
     *   For   *
     ***********/
    
    template <class Xpr>
    constexpr For<Xpr>::For(SizeT size, PtrId iptrId, SizeT step, const Xpr& xpr) :
	mSize(size),
	mIptrId(iptrId),
	mStep(step),
	mXpr(xpr),
	mExt(mXpr.rootSteps(mIptrId))
    {}

    template <class Xpr>
    template <class PosT1, class PosT2>
    inline SizeT For<Xpr>::operator()(const PosT1& mlast, const PosT2& last) const
    {
	for(SizeT i = 0; i != mSize; ++i){
	    const auto mpos = mlast + mStep * UPos(i);
	    const auto pos = last + mExt * UPos(i);
	    mXpr(mpos, pos);
	}
	return 0;
    }
	
    template <class Xpr>
    inline SizeT For<Xpr>::operator()() const
    {
	for(SizeT i = 0; i != mSize; ++i){
	    const SizeT mpos = mStep * UPos(i);
	    const auto pos = mExt * UPos(i);
	    mXpr(mpos, pos);
	}
	return 0;
    }

    template <class Xpr>
    inline auto For<Xpr>::rootSteps(PtrId ptrId) const
    {
	return mXpr.rootSteps(ptrId);
    }

    template <class Xpr>
    template <SizeT L>
    constexpr auto For<Xpr>::staticRootSteps(PtrId ptrId) const
    {
	return mXpr.template staticRootSteps<L>(ptrId);
    }


    /*************
     *   SLFor   *
     *************/

    template <SizeT N, SizeT L, SizeT S, class Xpr>
    constexpr SLFor<N,L,S,Xpr>::SLFor(PtrId iptrId, const Xpr& xpr) :
	mIptrId(iptrId),
	mXpr(xpr),
	mExt(mXpr.template staticRootSteps<L>(mIptrId))
    {}

    template <SizeT N, SizeT L, SizeT S, class Xpr>
    template <class PosT1, class PosT2>
    constexpr SizeT SLFor<N,L,S,Xpr>::operator()(const PosT1& mlast, const PosT2& last) const
    {
	return exec<0>(mlast, last);
    }
	
    template <SizeT N, SizeT L, SizeT S, class Xpr>
    constexpr SizeT SLFor<N,L,S,Xpr>::operator()() const
    {
	return exec<0>();
    }

    template <SizeT N, SizeT L, SizeT S, class Xpr>
    constexpr auto SLFor<N,L,S,Xpr>::rootSteps(PtrId ptrId) const
    {
	return mXpr.rootSteps(ptrId);
    }

    template <SizeT N, SizeT L, SizeT S, class Xpr>
    template <SizeT L2>
    constexpr auto SLFor<N,L,S,Xpr>::staticRootSteps(PtrId ptrId) const
    {
	return mXpr.template staticRootSteps<L2>(ptrId);
    }

    template <SizeT N, SizeT L, SizeT S, class Xpr>
    template <SizeT I, class PosT1, class PosT2>
    constexpr SizeT SLFor<N,L,S,Xpr>::exec(const PosT1& mlast, const PosT2& last) const
    {
	constexpr SPos<I> i;
	constexpr SPos<S> step;
	const auto mpos = mlast + step * i;
	const auto pos = last + mExt * i;
	mXpr(mpos, pos);
	if constexpr(I < N-1){
	    return exec<I+1>(mlast, last);
	}
	else {
	    return 0;
	}
    }

    template <SizeT N, SizeT L, SizeT S, class Xpr>
    template <SizeT I>
    constexpr SizeT SLFor<N,L,S,Xpr>::exec() const
    {
	constexpr SPos<I> i;
	constexpr SPos<S> step;
	const auto mpos = step * i;
	const auto pos = mExt * i;
	mXpr(mpos, pos);
	if constexpr(I < N-1){
	    return exec<I+1>();
	}
	else {
	    return 0;
	}
    }

    /************
     *   TFor   *
     ************/

    template <class Xpr>
    constexpr TFor<Xpr>::TFor(SizeT size, PtrId iptrId, SizeT step, const Xpr& xpr) :
	mSize(size),
	mIptrId(iptrId),
	mStep(step),
	mXpr(xpr),
	mExt(mXpr.rootSteps(mIptrId))
    {}

    template <class Xpr>
    template <class PosT1, class PosT2>
    inline SizeT TFor<Xpr>::operator()(const PosT1& mlast, const PosT2& last) const
    {
	int i = 0;
#pragma omp parallel shared(mXpr) private(i)
	{
	    auto xpr = mXpr;
#pragma omp for 
	    for(i = 0; i < mSize; i++){
		const UPos I(i);
		const auto mpos = mlast + mStep * I;
		const auto pos = last + mExt * I;
		mXpr(mpos, pos);
	    }
	}
	return 0;
    }
	
    template <class Xpr>
    inline SizeT TFor<Xpr>::operator()() const
    {
	int i = 0;
#pragma omp parallel shared(mXpr) private(i)
	{
	    auto xpr = mXpr;
#pragma omp for 
	    for(i = 0; i < static_cast<int>(mSize); i++){
		const UPos I(i);
		const auto mpos = mStep * I;
		const auto pos = mExt * I;
		mXpr(mpos, pos);
	    }
	}
	return 0;
    }

    template <class Xpr>
    inline auto TFor<Xpr>::rootSteps(PtrId ptrId) const
    {
	return mXpr.rootSteps(ptrId);
    }

    template <class Xpr>
    template <SizeT L2>
    inline auto TFor<Xpr>::staticRootSteps(PtrId ptrId) const
    {
	return mXpr.template staticRootSteps<L2>(ptrId);
    }


}

#endif
