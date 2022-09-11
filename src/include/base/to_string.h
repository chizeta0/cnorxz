
#ifndef __cxz_to_string_h__
#define __cxz_to_string_h__

#include "types.h"
#include <array>

namespace CNORXZ
{
    template <typename T>
    String toString(const T& a);

    template <typename T>
    String toString<vector<T>>(const vector& a);

    template <typename T, size_t N>
    String toString<std::array<T,N>>(const std::array<T,N>& a);

    template <>
    String toString<DType>(const DType& a);
}

#endif
