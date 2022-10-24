
#ifndef __cxz_reg_cc_h__
#define __cxz_reg_cc_h__

#include "reg.h"
#include "xpr/pos_type.h"

namespace CNORXZ
{
    template <typename T, class EPosT, SizeT... Is>
    decltype(auto) vregi(const T* d, const EPosT& pos, std::index_sequence<Is...> is)
    {
	constexpr SizeT N = epos_size<EPosT>::value;
	static_assert(N == sizeof...(Is), "got inconsistent index sequence");
	return PseudoReg<T,N> { d[pos.val()+pos.template get<Is>().val()]... };
    }
    
    template <typename T, class EPosT>
    decltype(auto) vreg(const T* d, const EPosT& pos)
    {
	constexpr SizeT N = epos_size<EPosT>::value;
	static_assert(is_epos_type<EPosT>::value, "got non-epos-type");
	if constexpr(pos_type_is_consecutive<EPosT>::value){
	    return *reinterpret_cast<const PseudoReg<T,N>*>
		(d+pos.val()+pos.template get<0>().val());
	}
	else {
	    return vregi(d, pos, std::make_index_sequence<N>{});
	}
    }

    template <typename T, class EPosT>
    decltype(auto) vreg(T* d, const EPosT& pos)
    {
	constexpr SizeT N = epos_size<EPosT>::value;
	static_assert(is_epos_type<EPosT>::value, "got non-epos-type");
	if constexpr(pos_type_is_consecutive<EPosT>::value){
	    return *reinterpret_cast<PseudoReg<T,N>*>
		(d+pos.val()+pos.template get<0>().val());
	}
	else {
	    return vregi(d, pos, std::make_index_sequence<N>{});
	}
    }

}

#endif
