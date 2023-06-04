
#ifndef __cxz_crange_cc_h__
#define __cxz_crange_cc_h__

#include "crange.h"
#include "index_mul.h"

namespace CNORXZ
{
    template <class Index>
    decltype(auto) CIndex::reformat(const Sptr<Index>& ind) const
    {
	return ind;
    }

    template <class Index>
    decltype(auto) CIndex::slice(const Sptr<Index>& ind) const
    {
	if(ind != nullptr){
	    if(ind->dim() != 0) {
		return Sptr<CIndex>();
	    }
	}
	return std::make_shared<CIndex>(*this);
    }

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
