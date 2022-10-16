
#ifndef __cxz_for_cc_h__
#define __cxz_for_cc_h__

#include <omp.h>

#include "for.h"

namespace CNORXZ
{
    /***********
     *   For   *
     ***********/
    
    template <SizeT L, class Xpr>
    constexpr For<L,Xpr>::For(SizeT size, const IndexId<L>& id, SizeT step, const Xpr& xpr) :
	mSize(size),
	mId(id),
	mStep(step),
	mXpr(xpr),
	mExt(mXpr.rootSteps(mId))
    {}

    template <SizeT L, class Xpr>
    template <class PosT1, class PosT2>
    inline SizeT For<L,Xpr>::operator()(const PosT1& mlast, const PosT2& last) const
    {
	for(SizeT i = 0; i != mSize; ++i){
	    const auto mpos = mlast + mStep * UPos(i);
	    const auto pos = last + mExt * UPos(i);
	    mXpr(mpos, pos);
	}
	return 0;
    }
	
    template <SizeT L, class Xpr>
    inline SizeT For<L,Xpr>::operator()() const
    {
	for(SizeT i = 0; i != mSize; ++i){
	    const SizeT mpos = mStep * UPos(i);
	    const auto pos = mExt * UPos(i);
	    mXpr(mpos, pos);
	}
	return 0;
    }

    template <SizeT L, class Xpr>
    template <SizeT I>
    inline decltype(auto) For<L,Xpr>::rootSteps(const IndexId<I>& id) const
    {
	return mXpr.rootSteps(id);
    }


    /*************
     *   SLFor   *
     *************/

    template <SizeT N, SizeT L, SizeT S, class Xpr>
    constexpr SLFor<N,L,S,Xpr>::SLFor(const IndexId<L>& id, const Xpr& xpr) :
	mId(id),
	mXpr(xpr),
	mExt(mXpr.RootSteps(mId))
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
    template <SizeT I>
    constexpr decltype(auto) SLFor<N,L,S,Xpr>::rootSteps(const IndexId<I>& id) const
    {
	return mXpr.rootSteps(id);
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

    template <SizeT L, class Xpr>
    constexpr TFor<L,Xpr>::TFor(SizeT size, const IndexId<L>& id, SizeT step, const Xpr& xpr) :
	mSize(size),
	mId(id),
	mStep(step),
	mXpr(xpr),
	mExt(mXpr.rootSteps(mId))
    {}

    template <SizeT L, class Xpr>
    template <class PosT1, class PosT2>
    inline SizeT TFor<L,Xpr>::operator()(const PosT1& mlast, const PosT2& last) const
    {
	int i = 0;
#pragma omp parallel shared(mXpr) private(i)
	{
	    auto xpr = mXpr;
#pragma omp for 
	    for(i = 0; i < mSize; i++){
		const auto mpos = mlast + mStep * UPos(i);
		const auto pos = last + mExt * UPos(i);
		mXpr(mpos, pos);
	    }
	}
	return 0;
    }
	
    template <SizeT L, class Xpr>
    inline SizeT TFor<L,Xpr>::operator()() const
    {
	int i = 0;
#pragma omp parallel shared(mXpr) private(i)
	{
	    auto xpr = mXpr;
#pragma omp for 
	    for(i = 0; i < static_cast<int>(mSize); i++){
		const auto mpos = mStep * UPos(i);
		const auto pos = mExt * UPos(i);
		mXpr(mpos, pos);
	    }
	}
	return 0;
    }

    template <SizeT L, class Xpr>
    template <SizeT I>
    inline decltype(auto) TFor<L,Xpr>::rootSteps(const IndexId<I>& id) const
    {
	return mXpr.rootSteps(id);
    }


}

#endif
