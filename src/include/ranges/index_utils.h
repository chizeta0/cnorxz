
#ifndef __cxz_index_utils_h__
#define __cxz_index_utils_h__

#include "base/base.h"

namespace CNORZX
{
    inline decltype(auto) getPack(const XIndexPtr& i);
    
    template <class F>
    inline decltype(auto) indexZip(const XIndexPtr& a, const XIndexPtr& b, F&& f);

    template <class F>
    inline decltype(auto) indexPackZip(const Vector<XIndexPtr>& a, const Vector<XIndexPtr>& b, F&& f);
}

#endif
