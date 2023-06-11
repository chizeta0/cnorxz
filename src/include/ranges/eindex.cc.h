
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

    template <typename MetaT, SizeT S, SizeT L, class I1>
    decltype(auto) operator*(const Sptr<EIndex<MetaT,S,L>>& a, const Sptr<I1>& b)
    {
	return iptrMul(a, b);
    }

    template <typename MetaT, SizeT S, SizeT L>
    decltype(auto) eindexPtr(const Sptr<LIndex<SIndex<MetaT,S>,L>>& i)
    {
	return std::make_shared<EIndex<MetaT,S,L>>(i);
    }

    template <SizeT L, typename MetaT, SizeT S>
    decltype(auto) eindexPtr(const Sptr<SIndex<MetaT,S>>& i)
    {
	return eindexPtr( lindexPtr<L>( i ) );
    }

    template <typename MetaT, SizeT S, SizeT L>
    decltype(auto) eindexPtr(const Sptr<SIndex<MetaT,S>>& i, CSizeT<L> l)
    {
	return eindexPtr<l>( i );
    }

    template <SizeT S, SizeT L1, SizeT L2, class Index>
    decltype(auto) eplex(const Sptr<Index>& i)
    {
	const SizeT isize = i->lmax().val();
	CXZ_ASSERT(isize % S == 0, "index max (= " << isize
		   << " ) not dividable by extension size = " << S);
	auto ci = std::make_shared<CIndex>( CRangeFactory(isize/S).create() );
	auto m = iter<0,S>([](auto i) { return i; },
			   [](const auto&... e) { return Arr<SizeT,S>{ e... }; } );
	auto ei = eindexPtr<L1>( std::make_shared<SIndex<SizeT,S>>
				 ( SRangeFactory<SizeT,S>(m).create() ) );
	if constexpr(L2 == 0){
	    return spackp(ci,ei);
	}
	else {
	    return spackp(lindexPtr<L2>(ci),ei);
	}
    }

    template <class Index, SizeT S, SizeT L>
    decltype(auto) eplex(const Sptr<Index>& i, CSizeT<S> s, CSizeT<L> l)
    {
	return eplex<s,l,0>(i);
    }

    template <class Index, SizeT S, SizeT L1, SizeT L2>
    decltype(auto) eplex(const Sptr<Index>& i, CSizeT<S> s, CSizeT<L1> l1, CSizeT<L2> l2)
    {
	return eplex<s,l1,l2>(i);
    }

}

#endif
