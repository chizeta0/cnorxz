
#ifndef __cxz_h5_types_h__
#define __cxz_h5_types_h__

namespace CNORXZ
{
    namespace hdf5
    {
	/********************
	 *   TypeIsNative   *
	 ********************/
	
	template <typename T>
	struct TypeIsNative { static constexpr bool value = false; };

	template <>
	struct TypeIsNative<SizeT> { static constexpr bool value = true; };

	template <typename T>
	struct TypeIsNative<Int> { static constexpr bool value = true; };

	template <typename T>
	struct TypeIsNative<Double> { static constexpr bool value = true; };

	/********************
	 *   NativeTypeId   *
	 ********************/

	template <typename T>
	struct NativeTypeId { static constexpr hid_t value = 0; };

	template <>
	struct NativeTypeId<SizeT> { static constexpr hid_t value = H5T_NATIVE_ULONG; };
	
	template <>
	struct NativeTypeId<Int> { static constexpr hid_t value = H5T_NATIVE_INT; };

	template <>
	struct NativeTypeId<Double> { static constexpr hid_t value = H5T_NATIVE_DOUBLE; };

	/**************
	 *   TypeId   *
	 **************/
 
	template <typename T>
	struct TypeId<T> { static inline hid_t get() { return 0; } };

	template <typename T, SizeT N>
	struct TypeId<Arr<T,N>> { static inline hid_t get(); };

	/*****************
	 *   getTypeId   *
	 *****************/

	template <typename T>
	hid_t getTypeId(const T& x);
    }
}

#endif
