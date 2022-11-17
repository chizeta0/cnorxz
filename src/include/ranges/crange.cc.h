
#ifndef __cxz_crange_cc_h__
#define __cxz_crange_cc_h__

#include "crange.h"

namespace CNORXZ
{
    template <class Xpr, class F>
    decltype(auto) CIndex::ifor(const Xpr& xpr, F&& f) const
    {
	return For<0,Xpr,F>(this->pmax(), this->id(), xpr, std::forward<F>(f));
    }
}

#endif
