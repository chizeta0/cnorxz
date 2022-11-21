
#ifndef __cxz_for_cc_h__
#define __cxz_for_cc_h__

#include <omp.h>

#include "for.h"

namespace CNORXZ
{

    /***********
     *   For   *
     ***********/
    
    template <SizeT L, class Xpr, class F>
    constexpr For<L,Xpr,F>::For(SizeT size, const IndexId<L>& id, const Xpr& xpr, F&& f) :
	mSize(size),
	mId(id),
	mXpr(xpr),
	mExt(mXpr.rootSteps(mId)),
	mF(f)
    {}

    template <SizeT L, class Xpr, class F>
    template <class PosT>
    inline decltype(auto) For<L,Xpr,F>::operator()(const PosT& last) const
    {
	if constexpr(std::is_same<F,NoF>::value){
	    for(SizeT i = 0; i != mSize; ++i){
		const auto pos = last + mExt * UPos(i);
		mXpr(pos);
	    }
	}
	else {
	    typedef typename
		std::remove_reference<decltype(mXpr(last + mExt * UPos(0)))>::type OutT;
	    auto o = OutT();
	    for(SizeT i = 0; i != mSize; ++i){
		const auto pos = last + mExt * UPos(i);
		mF(o, mXpr(pos));
	    }
	    return o;
	}
    }
	
    template <SizeT L, class Xpr, class F>
    inline decltype(auto) For<L,Xpr,F>::operator()() const
    {
	if constexpr(std::is_same<F,NoF>::value){
	    for(SizeT i = 0; i != mSize; ++i){
		const auto pos = mExt * UPos(i);
		mXpr(pos);
	    }
	}
	else {
	    typedef typename std::remove_reference<decltype(mXpr(mExt * UPos(0)))>::type OutT;
	    auto o = OutT();
	    for(SizeT i = 0; i != mSize; ++i){
		const auto pos = mExt * UPos(i);
		mF(o, mXpr(pos));
	    }
	    return o;
	}
    }

    template <SizeT L, class Xpr, class F>
    template <SizeT I>
    inline decltype(auto) For<L,Xpr,F>::rootSteps(const IndexId<I>& id) const
    {
	return mXpr.rootSteps(id);
    }

    /************************
     *   For (non-member)   *
     ************************/

    template <SizeT L, class Xpr, class F>
    constexpr decltype(auto) mkFor(SizeT size, const IndexId<L>& id, const Xpr& xpr, F&& f)
    {
	return For<L,Xpr,F>(size, id, xpr, std::forward<F>(f));
    }

    template <SizeT L, class Xpr>
    constexpr decltype(auto) mkFor(SizeT size, const IndexId<L>& id, const Xpr& xpr)
    {
	return For<L,Xpr>(size, id, xpr, NoF {});
    }

    /************
     *   SFor   *
     ************/

    template <SizeT N, SizeT L, class Xpr, class F>
    constexpr SFor<N,L,Xpr,F>::SFor(const IndexId<L>& id, const Xpr& xpr, F&& f) :
	mId(id),
	mXpr(xpr),
	mExt(mXpr.rootSteps(mId)),
	mF(f)
    {}

    template <SizeT N, SizeT L, class Xpr, class F>
    template <class PosT>
    constexpr decltype(auto) SFor<N,L,Xpr,F>::operator()(const PosT& last) const
    {
	if constexpr(std::is_same<F,NoF>::value){
	    exec2<0>(last);
	    return;
	}
	else {
	    return exec<0>(last);
	}
    }
	
    template <SizeT N, SizeT L, class Xpr, class F>
    constexpr decltype(auto) SFor<N,L,Xpr,F>::operator()() const
    {
	if constexpr(std::is_same<F,NoF>::value){
	    exec2<0>();
	    return;
	}
	else {
	    return exec<0>();
	}
    }

    template <SizeT N, SizeT L, class Xpr, class F>
    template <SizeT I>
    constexpr decltype(auto) SFor<N,L,Xpr,F>::rootSteps(const IndexId<I>& id) const
    {
	return mXpr.rootSteps(id);
    }

    template <SizeT N, SizeT L, class Xpr, class F>
    template <SizeT I, class PosT>
    constexpr decltype(auto) SFor<N,L,Xpr,F>::exec(const PosT& last) const
    {
	constexpr SPos<I> i;
	const auto pos = last + mExt * i;
	if constexpr(I < N-1){
	    return mF(mXpr(pos),exec<I+1>(last));
	}
	else {
	    return mXpr(pos);
	}
    }

    template <SizeT N, SizeT L, class Xpr, class F>
    template <SizeT I>
    constexpr decltype(auto) SFor<N,L,Xpr,F>::exec() const
    {
	constexpr SPos<I> i;
	const auto pos = mExt * i;
	if constexpr(I < N-1){
	    return mF(mXpr(pos),exec<I+1>());
	}
	else {
	    return mXpr(pos);
	}
    }

    template <SizeT N, SizeT L, class Xpr, class F>
    template <SizeT I, class PosT>
    inline void SFor<N,L,Xpr,F>::exec2(const PosT& last) const
    {
	constexpr SPos<I> i;
	const auto pos = last + mExt * i;
	if constexpr(I < N-1){
	    mXpr(pos);
	    exec2<I+1>(last);
	}
	else {
	    mXpr(pos);
	}
	return;
    }
    
    template <SizeT N, SizeT L, class Xpr, class F>
    template <SizeT I>
    inline void SFor<N,L,Xpr,F>::exec2() const
    {
	constexpr SPos<I> i;
	const auto pos = mExt * i;
	if constexpr(I < N-1){
	    mXpr(pos);
	    exec2<I+1>();
	}
	else {
	    mXpr(pos);
	}
	return;
    }

    /*************************
     *   SFor (non-member)   *
     *************************/

    template <SizeT N, SizeT L, class Xpr, class F>
    constexpr decltype(auto) mkSFor(const IndexId<L>& id, const Xpr& xpr, F&& f)
    {
	return SFor<N,L,Xpr,F>(id, xpr, std::forward<F>(f));
    }

    template <SizeT N, SizeT L, class Xpr>
    constexpr decltype(auto) mkSFor(const IndexId<L>& id, const Xpr& xpr)
    {
	return SFor<N,L,Xpr>(id, xpr, NoF {});
    }
    
    /************
     *   TFor   *
     ************/

    template <SizeT L, class Xpr, class F>
    constexpr TFor<L,Xpr,F>::TFor(SizeT size, const IndexId<L>& id, const Xpr& xpr, F&& f) :
	mSize(size),
	mId(id),
	mXpr(xpr),
	mExt(mXpr.rootSteps(mId)),
	mF(f)
    {
	// check for write access!!!
	/*
	if constexpr(is_static_pos_type<PosT>::value){
	    static_assert(step.val() != 0, "step has to be non-zero for TPos");
	}
	CXZ_ASSERT(step.val() != 0, "step has to be non-zero for TPos");
	*/
    }

    template <SizeT L, class Xpr, class F>
    template <class PosT>
    inline decltype(auto) TFor<L,Xpr,F>::operator()(const PosT& last) const
    {
	typedef typename std::remove_reference<decltype(mXpr(last + mExt * UPos(0)))>::type OutT;
	int i = 0;
	const int size = static_cast<int>(mSize);
	Vector<OutT> ov;
	if constexpr(not std::is_same<F,ZeroF>::value){
	    // replace if statement by "does s.th. non-trivial"
	    ov.resize(mSize);
	}
#pragma omp parallel private(i)
	{
	    auto xpr = mXpr;
#pragma omp for 
	    for(i = 0; i < size; i++){
		const auto pos = last + mExt * UPos(i);
		xpr(pos);
	    }
	}
	OutT o;
	if constexpr(not std::is_same<F,ZeroF>::value){
	    // replace if statement by "does s.th. non-trivial"
	    for(SizeT i = 0; i != mSize; ++i){
		mF(o, ov[i]);
	    }
	}
	return o;
    }
	
    template <SizeT L, class Xpr, class F>
    inline decltype(auto) TFor<L,Xpr,F>::operator()() const
    {
	typedef typename std::remove_reference<decltype(mXpr(mExt * UPos(0)))>::type OutT;
	int i = 0;
	const int size = static_cast<int>(mSize);
	Vector<OutT> ov;
	if constexpr(not std::is_same<F,ZeroF>::value){
	    // replace if statement by "does s.th. non-trivial"
	    ov.resize(mSize);
	}
#pragma omp parallel private(i)
	{
	    auto xpr = mXpr;
#pragma omp for 
	    for(i = 0; i < size; i++){
		const auto pos = mExt * UPos(i);
		xpr(pos);
	    }
	}
	OutT o;
	if constexpr(not std::is_same<F,ZeroF>::value){
	    // replace if statement by "does s.th. non-trivial"
	    for(SizeT i = 0; i != mSize; ++i){
		mF(o, ov[i]);
	    }
	}
	return o;
    }

    template <SizeT L, class Xpr, class F>
    template <SizeT I>
    inline decltype(auto) TFor<L,Xpr,F>::rootSteps(const IndexId<I>& id) const
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
    constexpr decltype(auto) EFor<N,L,Xpr>::operator()(const PosT& last) const
    {
	auto pos = mkEPos<N>(last, mExt);
	return mXpr(pos);
    }
	
    template <SizeT N, SizeT L, class Xpr>
    constexpr decltype(auto) EFor<N,L,Xpr>::operator()() const
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
