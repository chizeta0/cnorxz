// -*- C++ -*-
/**
   
   @file include/xpr/buf_xpr.h
   @brief Buffer xpression declarations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include "xpr_base.h"

namespace CNORXZ
{
    template <typename T, class IXprF>
    class BufXpr : public XprInterface<BufXpr>
    {
    public:
	BufXpr() = default;
	BufXpr(const BufXpr& in);
	BufXpr(BufXpr&& in);
	BufXpr& operator=(const BufXpr& in);
	BufXpr& operator=(BufXpr&& in);

	constexpr BufXpr(IXprF&& ixprf);

	template <class PosT>
	inline decltype(auto) operator()(const PosT& last) const;

	inline decltype(auto) operator()() const;

	template <SizeT I>
	inline decltype(auto) rootSteps(const IndexId<I>& id) const;

    private:
	T mBuf;
	IXprF mIXprF; // function to create set-buffer-xpr (reference to mBuf as arg)
	typedef decltype(mIXprF(mBuf)) IXpr;
	IXpr mIXpr;
    };

    template <typename T, class IXprF>
    constexpr decltype(auto) bufxpr(IXprF&& ixprf);
}
