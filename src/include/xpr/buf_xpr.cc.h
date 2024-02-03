// -*- C++ -*-
/**
   
   @file include/xpr/buf_xpr.cc.h
   @brief Buffer xpression template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include "buf_xpr.h"

namespace CNORXZ
{
    template <typename T, class IXprF>
    BufXpr<T,IXprF>::BufXpr(const BufXpr& in) :
	mBuf(in.mBuf),
	mIXprF(in.mIXprF),
	mIXpr(mIXprF(mBuf))
    {}

    template <typename T, class IXprF>
    BufXpr<T,IXprF>::BufXpr(BufXpr&& in) :
	mBuf(std::move(in.mBuf)),
	mIXprF(std::move(in.mIXprF)),
	mIXpr(mIXprF(mBuf))
    {}

    template <typename T, class IXprF>
    BufXpr& BufXpr<T,IXprF>::operator=(const BufXpr& in)
    {
	mBuf = in.mBuf;
	mIXprF = in.mIXprF;
	mIXpr = mIXprF(mBuf);
	return *this;
    }

    template <typename T, class IXprF>
    BufXpr& BufXpr<T,IXprF>::operator=(BufXpr&& in)
    {
	mBuf = std::move(in.mBuf);
	mIXprF = std::move(in.mIXprF);
	mIXpr = mIXprF(mBuf);
	return *this;
    }

    template <typename T, class IXprF>
    constexpr BufXpr<T,IXprF>::BufXpr(IXprF&& ixprf) :
	mIXprF(std::forward<IXprF>(ixprf)),
	mIXpr(mIXprF(mBuf))
    {}

    template <typename T, class IXprF>
    template <class PosT>
    inline decltype(auto) BufXpr<T,IXprF>::operator()(const PosT& last) const
    {
	mIXpr(last);
	return mBuf;
    }

    template <typename T, class IXprF>
    inline decltype(auto) BufXpr<T,IXprF>::operator()() const
    {
	mIXpr();
	return mBuf;
    }

    template <typename T, class IXprF>
    template <SizeT I>
    inline decltype(auto) BufXpr<T,IXprF>::rootSteps(const IndexId<I>& id) const
    {
	return mIXpr.rootSteps(id);
    }

    template <typename T, class IXprF>
    constexpr decltype(auto) bufxpr(IXprF&& ixprf)
    {
	return BufXpr<T,IXprF>( std::forward<IXprF>(ixprf) );
    }
}
