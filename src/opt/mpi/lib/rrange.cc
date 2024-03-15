// -*- C++ -*-
/**
   
   @file opt/mpi/lib/rrange.h
   @brief RRange and RIndex implementation.
   
   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include "rrange.h"
#include "rrange.cc.h"

namespace CNORXZ
{
    namespace mpi
    {

	/*===========================+
	 |    non-member functions   |
	 +===========================*/

	MArray<RangePtr> rsplit(const RangePtr& global, const RangePtr& geom)
	{
	    if(geom->dim() > 1){
		CXZ_ASSERT(global->dim() == geom->dim(),
			   "global range has to be of same dimension as geometry range, got "
			   << global->dim() << " and " << geom->dim());
		MArray<RangePtr> o(geom);
		YIndex k(geom);
		YIndex ke(geom, geom->size());
		auto mu = std::make_shared<CIndex>(geom->sub().range());
		MArray<MArray<RangePtr>> rs(geom->sub().range());
		rs(mu) = operation( [&](const SizeT i) { return rsplit(global->sub(i), geom->sub(i)); } , xpr(mu) );
		Vector<RangePtr> elem(geom->dim());
		for(; k != ke; ++k){
		    mu->ifor( operation( [&](const SizeT i, const MArray<RangePtr>& el){
			elem[i] = el[DIndex(k.pack()[i])];
		    }, xpr(mu), rs(mu) ), NoF {} )();
		    o[k] = YRangeFactory(elem).create();
		}
		return o;
	    }
	    else {
		CXZ_ASSERT(global->size() % geom->size() == 0,
			   "global range has to be dividible by geometry range, got "
			   << global->size() << " and " << geom->size());
		const SizeT n = global->size() / geom->size();
		auto k = std::make_shared<CIndex>(geom);
		auto jb = global->begin();
		auto je = global->begin();
		MArray<RangePtr> o(geom);
		o(k) = operation( [&](const SizeT x){ jb = n*x; je = n*(x+1)-1; return jb.prange(je); } , xpr(k) );
		return o;
	    }
	}
	
	RangePtr rrange(const RangePtr& global, const RangePtr& geom)
	{
	    const MArray<RangePtr> rs = rsplit(global, geom);
	    RangePtr o = nullptr;
	    for(auto ri = rs.begin(); ri != rs.end(); ++ri){
		if(ri.lex() == getRankNumber()){
		    o = *ri;
		}
	    }
	    assert(o);
	    auto loc = rangeCast<YRange>(global);
	    auto geo = rangeCast<YRange>(geom);
	    RRangeFactory<YRange,YRange> xx(loc, geo);
	    return RRangeFactory<YRange,YRange>(loc, geo).create();
	}
	
	
    } // namespace mpi
} // namespace CNORXZ
	
