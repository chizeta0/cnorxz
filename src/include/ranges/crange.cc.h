
#ifndef __cxz_crange_cc_h__
#define __cxz_crange_cc_h__

#include "crange.h"

namespace CNORXZ
{
    template <class Xpr>
    decltype(auto) CIndex::ifor(const Xpr& xpr) const
    {
	return For<0,Xpr>(this->max(), this->id(), xpr);
    }
}

#endif
