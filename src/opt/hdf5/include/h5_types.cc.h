
#ifndef __cxz_h5_types_cc_h__
#define __cxz_h5_types_cc_h__

namespace CNORXZ
{
    namespace hdf5
    {
	template <typename T, SizeT N>
	inline hid_t TypeId<Arr<T,N>>::get(Arr<T,N> x)
	{
	    static hid_t arrtype = H5Tarray_create2(getTypeId(x[0]), 1, N);
	    return arrtype;
	}

	template <typename T>
	hid_t getTypeId(T x)
	{
	    if constexpr (TypeIsNative<T>::value){
		return NativeTypeId<T>::value;
	    }
	    else {
		return TypeId<T>::get(x);
	    }
	}

    }
}

#endif
