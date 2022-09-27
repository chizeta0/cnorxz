
#ifndef __cxz_lindex_h__
#define __cxz_lindex_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"

namespace CNORXZ
{
    // static label to enforce loop unrolling
    template <class Index, SizeT L>
    class LIndex : public Index
    {
    public:
	typedef Index::IB IB;
	typedef Index::RangeType RangeType;

	DEFAULT_MEMBERS(LIndex);
	LIndex(const Index& i);
	LIndex(Index&& i);

	template <SizeT L1>
	auto getStaticStepSize(PtrId iptr) const;

    };
}

#endif
