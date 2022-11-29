
#ifndef __cxz_crange_cc_h__
#define __cxz_crange_cc_h__

#include "crange.h"
#include "index_mul.h"

namespace CNORXZ
{
    template <class Xpr, class F>
    decltype(auto) CIndex::ifor(const Xpr& xpr, F&& f) const
    {
	return For<0,Xpr,F>(this->pmax().val(), this->id(), xpr, std::forward<F>(f));
    }

    template <class I>
    decltype(auto) operator*(const Sptr<CIndex>& a, const Sptr<I>& b)
    {
	return iptrMul(a, b);
    }
}

#endif
