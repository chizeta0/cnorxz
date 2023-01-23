
#ifndef __cxz_h5_types_h__
#define __cxz_h5_types_h__

namespace CNORXZ
{
    namespace hdf5
    {

	/**************
	 *   TypeId   *
	 **************/
 
	template <typename T>
	struct TypeId { static inline hid_t get(); };

	template <>
	struct TypeId<SizeT> { static inline hid_t get(); };
	
	template <>
	struct TypeId<Int> { static inline hid_t get(); };

	template <>
	struct TypeId<Double> { static inline hid_t get(); };

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
