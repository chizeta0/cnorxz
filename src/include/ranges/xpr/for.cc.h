
#ifndef __cxz_for_cc_h__
#define __cxz_for_cc_h__

#include <omp.h>

#include "for.h"

namespace CNORXZ
{
    /***********
     *   For   *
     ***********/
    
    template <SizeT L, class PosT, class Xpr>
    constexpr For<L,PosT,Xpr>::For(SizeT size, const IndexId<L>& id, const PosT& step, const Xpr& xpr) :
	mSize(size),
	mId(id),
	mXpr(xpr),
	mStep(step),
	mExt(mXpr.rootSteps(mId))
    {
	static_assert(is_pos_type<PosT>::value, "got non-pos type");
    }

    template <SizeT L, class PosT, class Xpr>
    template <class PosT1, class PosT2>
    inline SizeT For<L,PosT,Xpr>::operator()(const PosT1& mlast, const PosT2& last) const
    {
	for(SizeT i = 0; i != mSize; ++i){
	    const auto mpos = mlast + mStep * UPos(i);
	    const auto pos = last + mExt * UPos(i);
	    mXpr(mpos, pos);
	}
	return 0;
    }
	
    template <SizeT L, class PosT, class Xpr>
    inline SizeT For<L,PosT,Xpr>::operator()() const
    {
	for(SizeT i = 0; i != mSize; ++i){
	    const auto mpos = mStep * UPos(i);
	    const auto pos = mExt * UPos(i);
	    mXpr(mpos, pos);
	}
	return 0;
    }

    template <SizeT L, class PosT, class Xpr>
    template <SizeT I>
    inline decltype(auto) For<L,PosT,Xpr>::rootSteps(const IndexId<I>& id) const
    {
	return mXpr.rootSteps(id);
    }


    /************
     *   SFor   *
     ************/

    template <SizeT N, SizeT L, class PosT, class Xpr>
    constexpr SFor<N,L,PosT,Xpr>::SFor(const IndexId<L>& id, const PosT& step, const Xpr& xpr) :
	mId(id),
	mStep(step),
	mXpr(xpr),
	mExt(mXpr.RootSteps(mId))
    {
	static_assert(is_pos_type<PosT>::value, "got non-pos type");
    }

    template <SizeT N, SizeT L, class PosT, class Xpr>
    template <class PosT1, class PosT2>
    constexpr SizeT SFor<N,L,PosT,Xpr>::operator()(const PosT1& mlast, const PosT2& last) const
    {
	return exec<0>(mlast, last);
    }
	
    template <SizeT N, SizeT L, class PosT, class Xpr>
    constexpr SizeT SFor<N,L,PosT,Xpr>::operator()() const
    {
	return exec<0>();
    }

    template <SizeT N, SizeT L, class PosT, class Xpr>
    template <SizeT I>
    constexpr decltype(auto) SFor<N,L,PosT,Xpr>::rootSteps(const IndexId<I>& id) const
    {
	return mXpr.rootSteps(id);
    }

    template <SizeT N, SizeT L, class PosT, class Xpr>
    template <SizeT I, class PosT1, class PosT2>
    constexpr SizeT SFor<N,L,PosT,Xpr>::exec(const PosT1& mlast, const PosT2& last) const
    {
	constexpr SPos<I> i;
	const auto mpos = mlast + mStep * i;
	const auto pos = last + mExt * i;
	mXpr(mpos, pos);
	if constexpr(I < N-1){
	    return exec<I+1>(mlast, last);
	}
	else {
	    return 0;
	}
    }

    template <SizeT N, SizeT L, class PosT, class Xpr>
    template <SizeT I>
    constexpr SizeT SFor<N,L,PosT,Xpr>::exec() const
    {
	constexpr SPos<I> i;
	const auto mpos = mStep * i;
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

    template <SizeT L, class PosT, class Xpr>
    constexpr TFor<L,PosT,Xpr>::TFor(SizeT size, const IndexId<L>& id, const PosT& step, const Xpr& xpr) :
	mSize(size),
	mId(id),
	mStep(step),
	mXpr(xpr),
	mExt(mXpr.rootSteps(mId))
    {
	static_assert(is_pos_type<PosT>::value, "got non-pos type");
	if constexpr(is_static_pos_type<PosT>::value){
	    static_assert(step.val() != 0, "step has to be non-zero for TPos");
	}
	CXZ_ASSERT(step.val() != 0, "step has to be non-zero for TPos");
    }

    template <SizeT L, class PosT, class Xpr>
    template <class PosT1, class PosT2>
    inline SizeT TFor<L,PosT,Xpr>::operator()(const PosT1& mlast, const PosT2& last) const
    {
	int i = 0;
	const int size = static_cast<int>(mSize);
#pragma omp parallel
	{
	    auto xpr = mXpr;
#pragma omp for 
	    for(i = 0; i < size; i++){
		const auto mpos = mlast + mStep * UPos(i);
		const auto pos = last + mExt * UPos(i);
		xpr(mpos, pos);
	    }
	}
	return 0;
    }
	
    template <SizeT L, class PosT, class Xpr>
    inline SizeT TFor<L,PosT,Xpr>::operator()() const
    {
	int i = 0;
	const int size = static_cast<int>(mSize);
#pragma omp parallel
	{
	    auto xpr = mXpr;
#pragma omp for 
	    for(i = 0; i < size; i++){
		const auto mpos = mStep * UPos(i);
		const auto pos = mExt * UPos(i);
		xpr(mpos, pos);
	    }
	}
	return 0;
    }

    template <SizeT L, class PosT, class Xpr>
    template <SizeT I>
    inline decltype(auto) TFor<L,PosT,Xpr>::rootSteps(const IndexId<I>& id) const
    {
	return mXpr.rootSteps(id);
    }


}

#endif
