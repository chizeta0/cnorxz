
#ifndef __cxz_eindex_h__
#define __cxz_eindex_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    
    template <typename MetaT, SizeT S, SizeT L>
    class EIndex : public LIndex<SIndex<MetaT,S>,L>
    {
    public:
	typedef typename LIndex<SIndex<MetaT,S>,L>::IB IB;
	typedef typename LIndex<SIndex<MetaT,S>,L>::RangeType RangeType;

	DEFAULT_MEMBERS(EIndex);
	EIndex(const LIndex<SIndex<MetaT,S>,L>& i);
	EIndex(LIndex<SIndex<MetaT,S>,L>&& i);

	template <class Xpr, class F>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;

    private:
	Sptr<LIndex<SIndex<MetaT,S>,L>> mLI;
    };

    template <typename MetaType, SizeT S, SizeT L, class I1>
    decltype(auto) operator*(const Sptr<EIndex<MetaType,S>>& a, const Sptr<I1>& b);

    template <typename MetaType, SizeT S, SizeT L>
    decltype(auto) eindexPtr(const Sptr<LIndex<SIndex<MetaT,S>,L>>& i);

    template <SizeT L, typename MetaType, SizeT S>
    decltype(auto) eindexPtr(const Sptr<SIndex<MetaT,S>>& i);

    template <typename MetaType, SizeT S, SizeT L>
    decltype(auto) eindexPtr(const Sptr<SIndex<MetaT,S>>& i, CSizeT<L> l);

    template <SizeT S, SizeT L1, SizeT L2, class Index>
    decltype(auto) eplex(const Sptr<Index>& i);

    template <class Index, SizeT S, SizeT L>
    decltype(auto) eplex(const Sptr<Index>& i, CSizeT<S> s, CSizeT<L> l);

    template <class Index, SizeT S, SizeT L1, SizeT L2>
    decltype(auto) eplex(const Sptr<Index>& i, CSizeT<S> s, CSizeT<L1> l1, CSizeT<L2> l2);

}

#endif
