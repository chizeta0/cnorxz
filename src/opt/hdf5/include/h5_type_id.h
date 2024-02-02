// -*- C++ -*-
/**

   @file opt/hdf5/include/h5_type_id.h
   @brief TypeId template declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_h5_type_id_h__
#define __cxz_h5_type_id_h__

#include "base/types.h"

namespace CNORXZ
{
    namespace hdf5
    {
	/** ****
	    Get a valid hdf5 id for a given type.
	    Return 0 if not implemented.
	    @tparam T The type.
	 */
	template <typename T>
	struct TypeId
	{
	    /** Get the type id. */
	    static inline hid_t get();
	};

	/** Specialization of TypeId for SizeT. */
	template <>
	struct TypeId<SizeT>
	{
	    /** Get the type id for SizeT. */
	    static inline hid_t get();
	};
	
	/** Specialization of TypeId for Int. */
	template <>
	struct TypeId<Int>
	{
	    /** Get the type id for Int. */
	    static inline hid_t get();
	};

	/** Specialization of TypeId for Double. */
	template <>
	struct TypeId<Double>
	{
	    /** Get the type id for Double. */
	    static inline hid_t get();
	};

	/** Specialization of TypeId for an array
	    @tparam T Element type.
	    @tparam N Array size.
	 */
	template <typename T, SizeT N>
	struct TypeId<Arr<T,N>>
	{
	    /** Get the type id for the given array. */
	    static inline hid_t get();
	};

	/** Wrapper function for TypeId::get()
	    @param Variable of given type.
	    @return A type id of the input variable's type.
	 */
	template <typename T>
	hid_t getTypeId(T x);
    }
}

#endif
