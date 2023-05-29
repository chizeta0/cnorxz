
#ifndef __cxz_eindex_h__
#define __cxz_eindex_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    template <typename MetaT, SizeT S>
    class EIndex : public SIndex<MetaT,S>
    {
    public:
	typedef typename SIndex<MetaT,S>::IB IB;
	typedef typename SIndex<MetaT,S>::RangeType RangeType;

	DEFAULT_MEMBERS(EIndex);
	EIndex(const SIndex<MetaT,S>& i);
	EIndex(SIndex<MetaT,S>&& i);

	template <class Xpr, class F>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;
    };
    
}

#endif
