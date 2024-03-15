// -*- C++ -*-
/**

   @file opt/mpi/include/mpi_wrappers.h
   @brief MPI wrapper template delarations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_wrappers_h__
#define __cxz_mpi_wrappers_h__

#include "mpi_base.h"
#include "typemap.h"

namespace CNORXZ
{
    namespace mpi
    {
	
	template <typename T>
	struct BCast
	{
	    static constexpr bool special = false;
	    static void bcast(T& d, SizeT root);
	};

	template <typename T>
	struct BCast<Vector<T>>
	{
	    static constexpr bool special = true;
	    static void bcast(Vector<T>& d, SizeT root);
	};

	template <typename T, SizeT N>
	struct BCast<Arr<T,N>>
	{
	    static constexpr bool special = true;
	    static void bcast(Arr<T,N>& d, SizeT root);
	};

	template <>
	struct BCast<String>
	{
	    static constexpr bool special = true;
	    static inline void bcast(String& d, SizeT root);
	};

	template <typename... Ts>
	struct BCast<Tuple<Ts...>>
	{
	    static constexpr bool special = true;
	    static void bcast(Tuple<Ts...>& d, SizeT root);
	};

	template <typename T>
	void bcast(T& d, SizeT root);


    } // namespace mpi
} // namespace CNORXZ

#endif
