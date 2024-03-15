// -*- C++ -*-

/**
   
   @file
   @brief

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_base_h__
#define __cxz_mpi_base_h__

#include "cnorxz.h"
#include "mpi.h"

namespace CNORXZ
{
    namespace mpi
    {
	// class declarations

	template <class RangeI, class RangeK>
	class RRange;

	template <class IndexI, class IndexK>
	class RIndex;
	
	// wrapper functions
	
	/** Get number of THIS rank. */
	SizeT getRankNumber();

	/** Get total number of ranks. */
	SizeT getNumRanks();
	
    } // namespace mpi
} // namespace CNORXZ

#endif
