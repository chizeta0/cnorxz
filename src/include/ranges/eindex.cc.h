
#ifndef __cxz_eindex_cc_h__
#define __cxz_eindex_cc_h__

#include "eindex.h"
#include "lindex.h"
#include "srange.h"

namespace CNORXZ
{
    
    template <typename MetaT, SizeT S, SizeT L>
    EIndex<MetaT,S,L>::EIndex(const Sptr<LIndex<SIndex<MetaT,S>,L>>& i) :
	LIndex<SIndex<MetaT,S>,L>(*i),
	mLI(i)
    {}

    template <typename MetaT, SizeT S, SizeT L>
    template <class Xpr, class F>
    decltype(auto) EIndex<MetaT,S,L>::ifor(const Xpr& xpr, F&& f) const
    {
	return EFor<S,L,Xpr>(mLI->id(), xpr, std::forward<F>(f));
    }

    template <typename MetaType, SizeT S, SizeT L, class I1>
    decltype(auto) operator*(const Sptr<EIndex<MetaT,S,L>>& a, const Sptr<I1>& b)
    {
	return iptrMul(a, b);
    }

    template <typename MetaT, SizeT S, SizeT L>
    decltype(auto) eindexPtr(const Sptr<LIndex<SIndex<MetaT,S>,L>>& i)
    {
	return std::make_shared<EIndex<MetaT,S>>(i);
    }

    template <SizeT L, typename MetaType, SizeT S>
    decltype(auto) eindexPtr(const Sptr<SIndex<MetaT,S>,L>& i)
    {
	return eindexPtr( lindexPtr<L>( i ) );
    }

    template <typename MetaType, SizeT S, SizeT L>
    decltype(auto) eindexPtr(const Sptr<SIndex<MetaT,S>,L>& i, CSizeT<L> l)
    {
	return eindexPtr<l>( i );
    }

}

#endif
