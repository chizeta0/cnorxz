
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
	    static MPI_Datatype value() { return MPI_BYTE; }
	};

	template <>
	struct Typemap<int>
	{
	    static constexpr bool exists = true;
	    static MPI_Datatype value() { return MPI_INT; }
	};

	template <>
	struct Typemap<unsigned>
	{
	    static constexpr bool exists = true;
	    static MPI_Datatype value() { return MPI_UNSIGNED; }
	};

	template <>
	struct Typemap<long int>
	{
	    static constexpr bool exists = true;
	    static MPI_Datatype value() { return MPI_LONG; }
	};

	template <>
	struct Typemap<unsigned long>
	{
	    static constexpr bool exists = true;
	    static MPI_Datatype value() { return MPI_UNSIGNED_LONG; }
	};

	template <>
	struct Typemap<double>
	{
	    static constexpr bool exists = true;
	    static MPI_Datatype value() { return MPI_DOUBLE; }
	};

	template <>
	struct Typemap<float>
	{
	    static constexpr bool exists = true;
	    static MPI_Datatype value() { return MPI_FLOAT; }
	};

	template <typename T, SizeT N>
	struct Typemap<Arr<T,N>>
	{
	    static constexpr bool exists = Typemap<T>::exists;
	    static MPI_Datatype value() { return Typemap<T>::value(); }
	};

	template <typename T>
	struct Typemap<Vector<T>>
	{
	    static constexpr bool exists = Typemap<T>::exists;
	    static MPI_Datatype value() { return Typemap<T>::value(); }
	};

	template <typename... Ts>
	struct Typemap<Tuple<Ts...>>
	{
	    static constexpr bool exists = ( Typemap<Ts>::exists and ... );
	    static MPI_Datatype value() { return MPI_BYTE; }
	};
	
	
	// further !!!
	
    } // namespace mpi
} // namespace CNORXZ

#endif
