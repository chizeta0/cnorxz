
#ifndef __cxz_h5_type_id_cc_h__
#define __cxz_h5_type_id_cc_h__

#include "base/types.h"

namespace CNORXZ
{
    namespace hdf5
    {
	template <SizeT N, typename... Ts>
	SizeT getTupleOffset(const Tuple<Ts...>& t, CSizeT<N> i)
	{
	    const PtrId beg = reinterpret_cast<PtrId>(&t);
	    const PtrId pos = reinterpret_cast<PtrId>(&std::get<i>(t));
	    return pos - beg;
	}

	/**************
	 *   TypeId   *
	 **************/

	template <typename T>
	inline hid_t TypeId<T>::get()
	{
	    return 0;
	}
	
	inline hid_t TypeId<SizeT>::get()
	{
	    return H5T_NATIVE_ULONG;
	}

	inline hid_t TypeId<Int>::get()
	{
	    return H5T_NATIVE_INT;
	}

	inline hid_t TypeId<Double>::get()
	{
	    return H5T_NATIVE_DOUBLE;
	}

	template <typename T, SizeT N>
	inline hid_t TypeId<Arr<T,N>>::get()
	{
	    static hid_t arrtype = H5Tarray_create2(TypeId<T>::get(), 1, N);
	    return arrtype;
	}

	/*****************
	 *   getTypeId   *
	 *****************/

	template <typename T>
	hid_t getTypeId(T x)
	{
	    return TypeId<T>::get();
	}

    }
}

#endif