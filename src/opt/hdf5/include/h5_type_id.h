
#ifndef __cxz_h5_type_id_h__
#define __cxz_h5_type_id_h__

#include "base/types.h"

namespace CNORXZ
{
    namespace hdf5
    {
	template <typename... Ts>
	decltype(auto) reverseTuple(const Tuple<Ts...>& t);

	template <typename... Ts>
	using RTuple = typename std::remove_reference<decltype(reverseTuple(Tuple<Ts...>()))>::type;
	template <SizeT I, typename... Ts>
	constexpr const auto& tget(const Tuple<Ts...>& t);
	
	template <SizeT I, typename... Ts>
	constexpr auto& tget(Tuple<Ts...>& t);
	
	template <SizeT N, typename... Ts>
	SizeT getTupleOffset(const Tuple<Ts...>& t, CSizeT<N> i);

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
	hid_t getTypeId(T x);
    }
}

#endif
