
#ifndef __cxz_index_utils_h__
#define __cxz_index_utils_h__

#include "base/base.h"

namespace CNORZX
{
    template <class Index>
    constexpr decltype(auto) getIndexDepth(const Index& ind);

    template <class Index, typename IntT>
    constexpr decltype(auto) getDimension(const Index& ind, IntT depth);

    
}

#endif
