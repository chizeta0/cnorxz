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
	auto six = *si;
	auto sie = si->range()->end();
	auto tix = *ti;
	const SizeT locsz = tix.local()->pmax().val();
	const SizeT tarsize = locsz*mpi::getNumRanks();
	const SizeT mapsize = m->size();
	const SizeT myrank = mpi::getRankNumber();
	if constexpr(mpi::is_rank_index<SrcIndex>::value){
	    CXZ_ASSERT(mapsize == six.local()->pmax().val(), "map not well-formatted: size = "
		       << mapsize << ", expected " << six.local()->pmax().val());
	    for(six = 0; six != sie; ++six){
		tix.at( f(*six) );
		if(six.rank() == myrank){
		    //const SizeT idx = (tix.pos() - locsz*tix.rank() + tarsize) % tarsize;
		    const SizeT idx = (tix.pos() - locsz*myrank + tarsize) % tarsize;
		    //const SizeT idx = tix.pos();
		    (*m)[six.local()->pos()] = idx;
		}
	    }
	}
	else {
	    CXZ_ASSERT(mapsize == six.pmax().val(), "map not well-formatted: size = "
		       << mapsize << ", expected " << six.pmax().val());
	    for(six = 0; six != sie; ++six){
		tix.at( f(*six) );
		//const SizeT idx = (tix.pos() - locsz*tix.rank() + tarsize) % tarsize;
		const SizeT idx = (tix.pos() - locsz*myrank + tarsize) % tarsize;
		//const SizeT idx = tix.pos()
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
	SizeT mapsize = 0;
	if constexpr(mpi::is_rank_index<SrcIndex>::value){
	    mapsize = si->local()->lmax().val();
	}
	else {
	    mapsize = si->lmax().val();
	}
	auto o = std::make_shared<Vector<SizeT>>(mapsize);
	setup(ti,si,f,o);
	return o;
    }
    
}

#endif
