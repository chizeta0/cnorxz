
#ifndef __cxz_reg_h__
#define __cxz_reg_h__

#include "base/base.h"

namespace CNORXZ
{
    // pseudo extension type to be returned if extension vector of
    // reuqired size is not available
    // no use of Arr = std::array here, since I want ensure that
    // it has exactly a memory size of N
    template <typename T, SizeT N>
    struct PseudoReg
    {
	T mD[N];
    };

    template <typename T, class EPosT, SizeT... Is>
    decltype(auto) vregi(const T* d, const EPosT& pos, std::index_sequence<Is...> is);

    // specialize for all kinds of available vector registers:
    template <typename T, class EPosT>
    decltype(auto) vreg(const T* d, const EPosT& pos);

    // specialize for all kinds of available vector registers:
    template <typename T, class EPosT>
    decltype(auto) vreg(T* d, const EPosT& pos);

    // TODO: Maybe specialize PseudoReg (-> Reg) itself (?)
}

#endif
