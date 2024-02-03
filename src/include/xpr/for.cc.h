// -*- C++ -*-
/**
   
   @file include/xpr/for.cc.h
   @brief For expressions template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

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
	if constexpr(std::is_same<typename std::remove_reference<F>::type,NoF>::value){
	    for(SizeT i = 0; i != mSize; ++i){
		const auto pos = last + mExt( UPos(i) );
		mXpr(pos);
	    }
	    return None {};
	}
	else {
	    typedef typename
		std::remove_reference<decltype(mXpr(last + mExt( UPos(0) )))>::type OutT;
	    auto o = OutT();
	    for(SizeT i = 0; i != mSize; ++i){
		const auto pos = last + mExt( UPos(i) );
		mF(o, mXpr(pos));
	    }
	    return o;
	}
    }
	
    template <SizeT L, class Xpr, class F>
    inline decltype(auto) For<L,Xpr,F>::operator()() const
    {
	if constexpr(std::is_same<typename std::remove_reference<F>::type,NoF>::value){
	    for(SizeT i = 0; i != mSize; ++i){
		const auto pos = mExt( UPos(i) );
		mXpr(pos);
	    }
	    return None {};
	}
	else {
	    typedef typename std::remove_reference<decltype(mXpr(mExt( UPos(0) )))>::type OutT;
	    auto o = OutT();
	    for(SizeT i = 0; i != mSize; ++i){
		const auto pos = mExt( UPos(i) );
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
	    return None {};
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
	    return None {};
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
	const auto pos = last + mExt( i );
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
	const auto pos = mExt( i );
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
	const auto pos = last + mExt( i );
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
	const auto pos = mExt( i );
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
     *   PFor   *
     ************/
    
    template <SizeT L1, SizeT L2, class Xpr, class F>
    constexpr PFor<L1,L2,Xpr,F>::PFor(SizeT size, const IndexId<L1>& id1, const IndexId<L2>& id2,
				      const SizeT* map, const Xpr& xpr, F&& f) :
	mSize(size),
	mId1(id1),
	mId2(id2),
	mXpr(xpr),
	mExt1(mXpr.rootSteps(mId1)),
	mExt2(mXpr.rootSteps(mId2)),
	mPart(1, map),
	mF(f)
    {}

    template <SizeT L1, SizeT L2, class Xpr, class F>
    template <class PosT>
    inline decltype(auto) PFor<L1,L2,Xpr,F>::operator()(const PosT& last) const
    {
	if constexpr(std::is_same<typename std::remove_reference<F>::type,NoF>::value){
	    for(SizeT i = 0; i != mSize; ++i){
		const auto pos1 = last + mExt1( UPos(i) );
		const auto pos2 = pos1 + mExt2( mPart( UPos(i) ) );
		mXpr(pos2);
	    }
	    return None {};
	}
	else {
	    typedef typename
		std::remove_reference<decltype(mXpr((last + mExt1( UPos(0) )) + mExt2( mPart( UPos(0) ) )))>::type OutT;
	    auto o = OutT();
	    for(SizeT i = 0; i != mSize; ++i){
		const auto pos1 = last + mExt1( UPos(i) );
		const auto pos2 = pos1 + mExt2( mPart( UPos(i) ) );
		mF(o, mXpr(pos2));
	    }
	    return o;
	}
    }
	
    template <SizeT L1, SizeT L2, class Xpr, class F>
    inline decltype(auto) PFor<L1,L2,Xpr,F>::operator()() const
    {
	if constexpr(std::is_same<typename std::remove_reference<F>::type,NoF>::value){
	    for(SizeT i = 0; i != mSize; ++i){
		const auto pos1 = mExt1( UPos(i) );
		const auto pos2 = pos1 + mExt2( mPart( UPos(i) ) );
		mXpr(pos2);
	    }
	    return None {};
	}
	else {
	    typedef typename std::remove_reference<decltype(mXpr(mExt1( UPos(0) ) + mExt2( mPart( UPos(0) ) )))>::type OutT;
	    auto o = OutT();
	    for(SizeT i = 0; i != mSize; ++i){
		const auto pos1 = mExt1( UPos(i) );
		const auto pos2 = pos1 + mExt2( mPart( UPos(i) ) );
		mF(o, mXpr(pos2));
	    }
	    return o;
	}
    }

    template <SizeT L1, SizeT L2, class Xpr, class F>
    template <SizeT I>
    inline decltype(auto) PFor<L1,L2,Xpr,F>::rootSteps(const IndexId<I>& id) const
    {
	return mXpr.rootSteps(id);
    }

    /*************************
     *   PFor (non-member)   *
     *************************/

    template <SizeT L1, SizeT L2, class Xpr, class F>
    constexpr decltype(auto) mkPFor(SizeT size, const IndexId<L1>& id1, const IndexId<L2>& id2,
				    const Xpr& xpr, F&& f)
    {
	return PFor<L1,L2,Xpr,F>(size, id1, id2, xpr, std::forward<F>(f));
    }

    template <SizeT L1, SizeT L2, class Xpr, class F>
    constexpr decltype(auto) mkPFor(SizeT size, const IndexId<L1>& id1, const IndexId<L2>& id2,
				    const Xpr& xpr)
    {
	return PFor<L1,L2,Xpr>(size, id1, id2, xpr, NoF {});
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
	typedef typename std::remove_reference<decltype(mXpr(last + mExt( UPos(0) )))>::type OutT;
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
		const auto pos = last + mExt( UPos(i) );
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
	typedef typename std::remove_reference<decltype(mXpr(mExt( UPos(0) )))>::type OutT;
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
		const auto pos = mExt( UPos(i) );
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

    template <SizeT N, SizeT L, class Xpr, class F>
    constexpr EFor<N,L,Xpr,F>::EFor(const IndexId<L>& id, const Xpr& xpr, F&& f) :
	mId(id),
	mXpr(xpr),
	mExt(mXpr.rootSteps(mId)),
	mF(std::forward<F>(f))
    {}

    template <SizeT N, SizeT L, class Xpr, class F>
    template <class PosT>
    constexpr decltype(auto) EFor<N,L,Xpr,F>::operator()(const PosT& last) const
    {
	if constexpr(std::is_same<typename std::remove_reference<F>::type,NoF>::value){
	    const auto pos = mkEPos<N>(last, mExt);
	    mXpr(pos);
	    return None {};
	}
	else {
	    typedef typename
		std::remove_reference<decltype(mXpr(mkEPos<N>(SPos<0>(), mExt)))>::type OutT;
	    auto o = OutT();
	    const auto pos = mkEPos<N>(last, mExt);
	    mF(o, mXpr(pos));
	    return o;
	}
    }
	
    template <SizeT N, SizeT L, class Xpr, class F>
    constexpr decltype(auto) EFor<N,L,Xpr,F>::operator()() const
    {
	if constexpr(std::is_same<typename std::remove_reference<F>::type,NoF>::value){
	    const auto pos = mkEPos<N>(SPos<0>(), mExt);
	    mXpr(pos);
	    return None {};
	}
	else {
	    typedef typename
		std::remove_reference<decltype(mXpr(mkEPos<N>(SPos<0>(), mExt)))>::type OutT;
	    auto o = OutT();
	    const auto pos = mkEPos<N>(SPos<0>(), mExt);
	    mF(o, mXpr(pos));
	    return o;
	}
    }

    template <SizeT N, SizeT L, class Xpr, class F>
    template <SizeT I>
    constexpr decltype(auto) EFor<N,L,Xpr,F>::rootSteps(const IndexId<I>& id) const
    {
	return mXpr.rootSteps(id);
    }
}

#endif
