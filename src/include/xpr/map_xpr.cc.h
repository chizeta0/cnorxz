// -*- C++ -*-
/**
   
   @file include/xpr/map_xpr.cc.h
   @brief Map xpression template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_map_xpr_cc_h__
#define __cxz_map_xpr_cc_h__

#include "map_xpr.h"

namespace CNORXZ
{
    template <class TarIndex, class SrcIndex, class F>
    static void setupMap(const Sptr<TarIndex>& ti, const Sptr<SrcIndex>& si,
			 const F& f, const Sptr<Vector<SizeT>>& m)
    {
	auto six = *si;
	auto sie = si->range()->end();
	auto tix = *ti;
	for(six = 0; six != sie; ++six){
	    tix.at( f(*six) );
	    if(six.rank() == getRankNumber()){
		(*m)[six.local()->lex()] = tix.pos();
	    }
	}
    }

    template <class TarIndex, class SrcIndex, class F>
    static Sptr<Vector<SizeT>> setupMap(const Sptr<TarIndex>& ti, const Sptr<SrcIndex>& si,
					const F& f)
    {
	auto o = std::make_shared<Vector<SizeT>>(si->local()->lmax().val());
	setupMap(ti,si,f,o);
	return o;
    }

    template <class TarIndex, class SrcIndex, class Xpr>
    template <class F>
    MapXpr<TarIndex,SrcIndex,Xpr>::MapXpr(const Sptr<TarIndex>& ti, const Sptr<SrcIndex>& si,
					  const F& f, Xpr&& xpr) :
	mTi(ti), mSi(si),
	mMap(std::make_shared<Vector<SizeT>>(mSi->local()->lmax().val())),
	mXpr(std::forward<Xpr>(xpr)),
	mExt(mkFPos( mXpr.rootSteps(mTi->id()), mMap->data() ))
    {
	setupMap(ti,si,f,mMap);
    }

    template <class TarIndex, class SrcIndex, class Xpr>
    MapXpr<TarIndex,SrcIndex,Xpr>::MapXpr(const Sptr<TarIndex>& ti, const Sptr<SrcIndex>& si,
					  const Sptr<Vector<SizeT>>& m, Xpr&& xpr) :
	mTi(ti), mSi(si),
	mMap(m), mXpr(std::forward<Xpr>(xpr)),
	mExt(mkFPos( mXpr.rootSteps(mTi->id()), mMap->data() ))
    {}
	    

    template <class TarIndex, class SrcIndex, class Xpr>
    template <class PosT>
    decltype(auto) MapXpr<TarIndex,SrcIndex,Xpr>::operator()(const PosT& last) const
    {
	return mXpr( last.next() + mExt( last ) );
    }

    template <class TarIndex, class SrcIndex, class Xpr>
    decltype(auto) MapXpr<TarIndex,SrcIndex,Xpr>::operator()() const
    {
	return mXpr( mExt( UPos(0) ) );
    }

    template <class TarIndex, class SrcIndex, class Xpr>
    template <SizeT I>
    decltype(auto) MapXpr<TarIndex,SrcIndex,Xpr>::rootSteps(const IndexId<I>& id) const
    {
	return mSi->stepSize(id) << mXpr.rootSteps(id);
    }

    template <class TarIndex, class SrcIndex, class F, class Xpr>
    decltype(auto) mapXpr(const Sptr<TarIndex>& ti, const Sptr<SrcIndex>& si, const F& f, Xpr&& xpr)
    {
	return MapXpr<TarIndex,SrcIndex,Xpr>(ti,si,f,std::forward<Xpr>(xpr));
    }

    template <class TarIndex, class SrcIndex, class Xpr>
    decltype(auto) mapXpr(const Sptr<TarIndex>& ti, const Sptr<SrcIndex>& si,
			  const Sptr<Vector<SizeT>>& m, Xpr&& xpr)
    {
	return MapXpr<TarIndex,SrcIndex,Xpr>(ti,si,m,std::forward<Xpr>(xpr));
    }

}

#endif
