// -*- C++ -*-
/**

   @file opt/include/rmap_xpr.cc.h
   @brief MPI specific specializations for MapXpr

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
 **/

#ifndef __cxz_mpi_rmap_xpr_cc_h__
#define __cxz_mpi_rmap_xpr_cc_h__

#include "rmap_xpr.h"
#include "mpi_base.h"

namespace CNORXZ
{
    template <class TarI, class RTarI, class SrcIndex, class F>
    void
    MapSetup<mpi::RIndex<TarI,RTarI>,SrcIndex,F>::setup(const Sptr<mpi::RIndex<TarI,RTarI>>& ti,
							const Sptr<SrcIndex>& si,
							const F& f, const Sptr<Vector<SizeT>>& m)
    {
	// This was the old shift, keep it here as comment if we want to introduce other shifts
	// in order to reduce memory consumption by the maps;
	// remember to invert the shift in the map xpr BEFORE calling the map!
	//const SizeT locsz = tix.local()->pmax().val();
	//const SizeT tarsize = locsz*mpi::getNumRanks();
	//const SizeT idx = (tix.pos() - locsz*myrank + tarsize) % tarsize;

	auto six = *si;
	auto sie = si->range()->end();
	auto tix = *ti;
	const SizeT mapsize = m->size();
	const SizeT myrank = mpi::getRankNumber();
	if constexpr(mpi::is_rank_index<SrcIndex>::value){
	    CXZ_ASSERT(mapsize == six.pmax().val(), "map not well-formatted: size = "
		       << mapsize << ", expected " << six.local()->pmax().val());
	    for(six = 0; six != sie; ++six){
		tix.at( f(*six) );
		if(six.rank() == myrank){
		    const SizeT idx = tix.pos();
		    (*m)[six.pos()] = idx;
		}
	    }
	}
	else {
	    CXZ_ASSERT(mapsize == six.pmax().val(), "map not well-formatted: size = "
		       << mapsize << ", expected " << six.pmax().val());
	    for(six = 0; six != sie; ++six){
		tix.at( f(*six) );
		const SizeT idx = tix.pos()
		(*m)[six.pos()] = idx;
	    }
	}
    }

    template <class TarI, class RTarI, class SrcIndex, class F>
    Sptr<Vector<SizeT>>
    MapSetup<mpi::RIndex<TarI,RTarI>,SrcIndex,F>::setup(const Sptr<mpi::RIndex<TarI,RTarI>>& ti,
							const Sptr<SrcIndex>& si,
							const F& f)
    {
	const SizeT mapsize = si->pmax().val();
	auto o = std::make_shared<Vector<SizeT>>(mapsize);
	setup(ti,si,f,o);
	return o;
    }
    
}

#endif
