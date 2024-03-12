
#ifndef __cxz_mpi_typemap_h__
#define __cxz_mpi_typemap_h__

#include "mpi.h"

namespace CNORXZ
{
    namespace mpi
    {
	template <typename T>
	struct Typemap
	{
	    static constexpr bool exists = false;
	};

	template <>
	struct Typemap<int>
	{
	    static constexpr bool exists = true;
	    static constexpr MPI_Datatype value = MPI_INT;
	};

	template <>
	struct Typemap<unsigned>
	{
	    static constexpr bool exists = true;
	    static constexpr MPI_Datatype value = MPI_UNSIGNED;
	};

	template <>
	struct Typemap<long int>
	{
	    static constexpr bool exists = true;
	    static constexpr MPI_Datatype value = MPI_LONG;
	};

	template <>
	struct Typemap<unsigned long>
	{
	    static constexpr bool exists = true;
	    static constexpr MPI_Datatype value = MPI_UNSIGNED_LONG;
	};

	template <>
	struct Typemap<double>
	{
	    static constexpr bool exists = true;
	    static constexpr MPI_Datatype value = MPI_DOUBLE;
	};

	template <>
	struct Typemap<float>
	{
	    static constexpr bool exists = true;
	    static constexpr MPI_Datatype value = MPI_FLOAT;
	};

	// further !!!
	
    } // namespace mpi
} // namespace CNORXZ

#endif
