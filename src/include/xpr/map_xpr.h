// -*- C++ -*-
/**

   @file include/xpr/map_xpr.h
   @brief Map expression declarations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
 **/

#ifndef __cxz_map_xpr_h__
#define __cxz_map_xpr_h__

#include "xpr_base.h"

namespace CNORXZ
{
    template <class TarIndex, class SrcIndex, class F>
    static void setupMap(const Sptr<TarIndex>& ti, const Sptr<SrcIndex>& si,
			 const F& f, const Sptr<Vector<SizeT>>& m);

    template <class TarIndex, class SrcIndex, class F>
    static Sptr<Vector<SizeT>> setupMap(const Sptr<TarIndex>& ti, const Sptr<SrcIndex>& si,
					const F& f);

    template <class TarIndex, class SrcIndex, class Xpr>
    class MapXpr : public XprInterface<MapXpr<TarIndex,SrcIndex,Xpr>>
    {
    private:
	Sptr<TarIndex> mTi;
	Sptr<SrcIndex> mSi;
	Sptr<Vector<SizeT>> mMap;
	Xpr mXpr;
	typedef decltype(mkFPos( mXpr.rootSteps(mTi->id()), mMap->data() )) Ext;
	Ext mExt;
    
    public:
	
	MapXpr() = default;

	// src local
	template <class F>
	MapXpr(const Sptr<TarIndex>& ti, const Sptr<SrcIndex>& si, const F& f, Xpr&& xpr);

	MapXpr(const Sptr<TarIndex>& ti, const Sptr<SrcIndex>& si,
	       const Sptr<Vector<SizeT>>& m, Xpr&& xpr);
	    

	template <class PosT>
	decltype(auto) operator()(const PosT& last) const;

	decltype(auto) operator()() const;

	template <SizeT I>
	decltype(auto) rootSteps(const IndexId<I>& id) const;
    };

    template <class TarIndex, class SrcIndex, class F, class Xpr>
    decltype(auto) mapXpr(const Sptr<TarIndex>& ti, const Sptr<SrcIndex>& si, const F& f, Xpr&& xpr);

    template <class TarIndex, class SrcIndex, class Xpr>
    decltype(auto) mapXpr(const Sptr<TarIndex>& ti, const Sptr<SrcIndex>& si,
			  const Sptr<Vector<SizeT>>& m, Xpr&& xpr);
    
   
}

#endif
