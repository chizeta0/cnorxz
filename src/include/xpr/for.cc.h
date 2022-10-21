
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
    constexpr For<L,Xpr>::For(SizeT size, const IndexId<L>& id, const Xpr& xpr) :
	mSize(size),
	mId(id),
	mXpr(xpr),
	mExt(mXpr.rootSteps(mId))
    {}

    template <SizeT L, class Xpr>
    template <class PosT>
    inline SizeT For<L,Xpr>::operator()(const PosT& last) const
    {
	for(SizeT i = 0; i != mSize; ++i){
	    const auto pos = last + mExt * UPos(i);
	    mXpr(pos);
	}
	return 0;
    }
	
    template <SizeT L, class Xpr>
    inline SizeT For<L,Xpr>::operator()() const
    {
	for(SizeT i = 0; i != mSize; ++i){
	    const auto pos = mExt * UPos(i);
	    mXpr(pos);
	}
	return 0;
    }

    template <SizeT L, class Xpr>
    template <SizeT I>
    inline decltype(auto) For<L,Xpr>::rootSteps(const IndexId<I>& id) const
    {
	return mXpr.rootSteps(id);
    }


    /************
     *   SFor   *
     ************/

    template <SizeT N, SizeT L, class Xpr>
    constexpr SFor<N,L,Xpr>::SFor(const IndexId<L>& id, const Xpr& xpr) :
	mId(id),
	mXpr(xpr),
	mExt(mXpr.RootSteps(mId))
    {}

    template <SizeT N, SizeT L, class Xpr>
    template <class PosT>
    constexpr SizeT SFor<N,L,Xpr>::operator()(const PosT& last) const
    {
	return exec<0>(last);
    }
	
    template <SizeT N, SizeT L, class Xpr>
    constexpr SizeT SFor<N,L,Xpr>::operator()() const
    {
	return exec<0>();
    }

    template <SizeT N, SizeT L, class Xpr>
    template <SizeT I>
    constexpr decltype(auto) SFor<N,L,Xpr>::rootSteps(const IndexId<I>& id) const
    {
	return mXpr.rootSteps(id);
    }

    template <SizeT N, SizeT L, class Xpr>
    template <SizeT I, class PosT>
    constexpr SizeT SFor<N,L,Xpr>::exec(const PosT& last) const
    {
	constexpr SPos<I> i;
	const auto pos = last + mExt * i;
	mXpr(pos);
	if constexpr(I < N-1){
	    return exec<I+1>(last);
	}
	else {
	    return 0;
	}
    }

    template <SizeT N, SizeT L, class Xpr>
    template <SizeT I>
    constexpr SizeT SFor<N,L,Xpr>::exec() const
    {
	constexpr SPos<I> i;
	const auto pos = mExt * i;
	mXpr(pos);
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
    constexpr TFor<L,Xpr>::TFor(SizeT size, const IndexId<L>& id, const Xpr& xpr) :
	mSize(size),
	mId(id),
	mXpr(xpr),
	mExt(mXpr.rootSteps(mId))
    {
	// check for write access!!!
	/*
	if constexpr(is_static_pos_type<PosT>::value){
	    static_assert(step.val() != 0, "step has to be non-zero for TPos");
	}
	CXZ_ASSERT(step.val() != 0, "step has to be non-zero for TPos");
	*/
    }

    template <SizeT L, class Xpr>
    template <class PosT>
    inline SizeT TFor<L,Xpr>::operator()(const PosT& last) const
    {
	int i = 0;
	const int size = static_cast<int>(mSize);
#pragma omp parallel private(i)
	{
	    auto xpr = mXpr;
#pragma omp for 
	    for(i = 0; i < size; i++){
		const auto pos = last + mExt * UPos(i);
		xpr(pos);
	    }
	}
	return 0;
    }
	
    template <SizeT L, class Xpr>
    inline SizeT TFor<L,Xpr>::operator()() const
    {
	int i = 0;
	const int size = static_cast<int>(mSize);
#pragma omp parallel private(i)
	{
	    auto xpr = mXpr;
#pragma omp for 
	    for(i = 0; i < size; i++){
		const auto pos = mExt * UPos(i);
		xpr(pos);
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


    /************
     *   EFor   *
     ************/

    template <SizeT N, SizeT L, class Xpr>
    constexpr EFor<N,L,Xpr>::EFor(const IndexId<L>& id, const Xpr& xpr) :
	mId(id),
	mXpr(xpr),
	mExt(mXpr.RootSteps(mId))
    {}

    template <SizeT N, SizeT L, class Xpr>
    template <class PosT>
    constexpr SizeT EFor<N,L,Xpr>::operator()(const PosT& last) const
    {
	auto pos = mkEPos<N>(last, mExt);
	return mXpr(pos);
    }
	
    template <SizeT N, SizeT L, class Xpr>
    constexpr SizeT EFor<N,L,Xpr>::operator()() const
    {
	auto pos = mkEPos<N>(SPos<0>(), mExt);
	return mXpr(pos);
    }

    template <SizeT N, SizeT L, class Xpr>
    template <SizeT I>
    constexpr decltype(auto) EFor<N,L,Xpr>::rootSteps(const IndexId<I>& id) const
    {
	return mXpr.rootSteps(id);
    }
}

#endif
