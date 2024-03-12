// -*- C++ -*-
/**

   @file opt/mpi/include/mpi_wrappers.cc.h
   @brief MPI wrapper template implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_wrappers_cc_h__
#define __cxz_mpi_wrappers_cc_h__

#include "mpi_wrappers.h"

namespace CNORXZ
{
    namespace mpi
    {
	
	template <typename T>
	void BCast<T>::bcast(T& d, SizeT root)
	{
	    static_assert( TypeMap<T>::exists, "no bcast implementation for given type" );
	    const int ret = MPI_Bcast( reinterpret_cast<void*>(&d), 1,
				       TypeMap<T>::value, MPI_COMM_WORLD );
	    CXZ_ASSERT(ret == MPI_SUCCESS, "got bcast error = " << ret);
	    return;
	}

	// preliminary solutions, use cereal serialization in the future!!!
	template <typename T>
	void BCast<Vector<T>>::bcast(Vector<T>& d, SizeT root)
	{
	    SizeT size = d.size();
	    const int ret = MPI_Bcast( reinterpret_cast<void*>(&size), 1,
				       MPI_UNSIGNED_LONG, MPI_COMM_WORLD );
	    CXZ_ASSERT(ret == MPI_SUCCESS, "got bcast error = " << ret);
	    if(size != d.size()){
		d.resize(size);
	    }
	    if constexpr( BCast<T>::special ){
		for(auto& x: d){
		    bcast(x, root);
		}
	    }
	    else {
		const int ret2 = MPI_Bcast( reinterpret_cast<void*>(d.data()), size,
					    TypeMap<T>::value, MPI_COMM_WORLD );
		CXZ_ASSERT(ret2 == MPI_SUCCESS, "got bcast error = " << ret2);
	    }
	}

	template <typename T, SizeT N>
	void BCast<Arr<T,N>>::bcast(Arr<T,N>& d, SizeT root)
	{
	    if constexpr( BCast<T,N>::special ){
		for(auto& x: d){
		    bcast(x, root);
		}
	    }
	    else {
		const int ret = MPI_Bcast( reinterpret_cast<void*>(d.data()), N,
					   TypeMap<T>::value, MPI_COMM_WORLD );
		CXZ_ASSERT(ret == MPI_SUCCESS, "got bcast error = " << ret2);
	    }
	}

	inline void BCast<String>::bcast(String& d, SizeT root)
	{
	    SizeT size = d.size();
	    const int ret = MPI_Bcast( reinterpret_cast<void*>(&size), 1,
				       MPI_UNSIGNED_LONG, MPI_COMM_WORLD );
	    CXZ_ASSERT(ret == MPI_SUCCESS, "got bcast error = " << ret);
	    if(size != d.size()){
		d.resize(size);
	    }
	    const int ret = MPI_Bcast( reinterpret_cast<void*>(d.data()), size,
				       MPI_CHAR, MPI_COMM_WORLD );
	    CXZ_ASSERT(ret == MPI_SUCCESS, "got bcast error = " << ret);
	}

	template <typename... Ts>
	void BCast<Tuple<Ts...>>::bcast(Tuple<Ts...>& d, SizeT root)
	{
	    if constexpr( ( BCast<Ts>::special or ... ) ){
		ifor<0,sizeof...(Ts)>( [&](auto i) { bcast( std::get<i>(d), root ); }, NoF {} );
	    }
	    else {
		const int ret = MPI_Bcast( reinterpret_cast<void*>(&d), sizeof(d),
					   MPI_BYTE, MPI_COMM_WORLD );
		CXZ_ASSERT(ret == MPI_SUCCESS, "got bcast error = " << ret);
	    }
	}

	template <typename T>
	void bcast(T& d, SizeT root)
	{
	    return BCast<T>(d, root);
	}
	
    } // namespace mpi
} // namespace CNORXZ

#endif
