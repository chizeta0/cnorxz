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

namespace CNORXZ
{
    template <class TarIndex, class SrcI, class RSrcI, class F>
    void
    MapSetup<TarIndex,mpi::RIndex<SrcI,RSrcI>,F>::setup(const Sptr<TarIndex>& ti,
							const Sptr<mpi::RIndex<SrcI,RSrcI>>& si,
							const F& f, const Sptr<Vector<SizeT>>& m)
    {
	auto six = *si;
	auto sie = si->range()->end();
	auto tix = *ti;
	for(six = 0; six != sie; ++six){
	    tix.at( f(*six) );
	    if(six.rank() == mpi::getRankNumber()){
		(*m)[six.local()->lex()] = tix.pos();
	    }
	}
    }

    template <class TarIndex, class SrcI, class RSrcI, class F>
    Sptr<Vector<SizeT>>
    MapSetup<TarIndex,mpi::RIndex<SrcI,RSrcI>,F>::setup(const Sptr<TarIndex>& ti,
							const Sptr<mpi::RIndex<SrcI,RSrcI>>& si,
							const F& f)
    {
	auto o = std::make_shared<Vector<SizeT>>(si->local()->lmax().val());
	setup(ti,si,f,o);
	return o;
    }
    
}

#endif
