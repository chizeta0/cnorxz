
#ifndef __cxz_index_utils_h__
#define __cxz_index_utils_h__

#include "base/base.h"
#include "index_base.h"

namespace CNORXZ
{
    // automatically set static format if SIndices are used!
    template <class... Indices>
    constexpr decltype(auto) autoiPtr(const SPack<Indices...>& pack);

    inline SPtr<YIndex> autoiPtr(const DPack& pack);
    
}

#endif