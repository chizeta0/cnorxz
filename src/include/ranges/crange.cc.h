
#ifndef __cxz_crange_cc_h__
#define __cxz_crange_cc_h__

#include "crange.h"

namespace CNORXZ
{
    template <class PosT, class Xpr>
    decltype(auto) CIndex::ifor(const PosT& step, const Xpr& xpr) const
    {
	return For<0,PosT,Xpr>(this->max(), this->id(), step, xpr);
    }
}

#endif
