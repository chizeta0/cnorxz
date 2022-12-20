
#ifndef __cxz_lindex_h__
#define __cxz_lindex_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    // static label to enforce loop unrolling
    template <class Index, SizeT L>
    class LIndex : public Index
    {
    public:
	typedef typename Index::IB IB;
	typedef typename Index::RangeType RangeType;

	DEFAULT_MEMBERS(LIndex);
	LIndex(const Index& i);
	LIndex(Index&& i);

	IndexId<L> id() const;
	
	template <SizeT I>
	decltype(auto) stepSize(const IndexId<I>& id) const;

    };
}

#endif
