// -*- C++ -*-
/**

   @file opt/include/rmap_xpr.h
   @brief MPI specific specializations for MapXpr

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
 **/

#ifndef __cxz_mpi_rmap_xpr_h__
#define __cxz_mpi_rmap_xpr_h__

#include "xpr/map_xpr.h"
#include "rrange.h"

namespace CNORXZ
{
    template <class TarI, class RTarI, class SrcIndex, class F>
    struct MapSetup<mpi::RIndex<TarI,RTarI>,SrcIndex,F>
    {
	static void setup(const Sptr<mpi::RIndex<TarI,RTarI>>& ti,
			  const Sptr<SrcIndex>& si,
			  const F& f, const Sptr<Vector<SizeT>>& m);

	static Sptr<Vector<SizeT>> setup(const Sptr<mpi::RIndex<TarI,RTarI>>& ti,
					 const Sptr<SrcIndex>& si,
					 const F& f);
    };
}

#endif
