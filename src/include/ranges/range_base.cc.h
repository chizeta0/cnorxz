
#ifndef __cxz_range_base_cc_h__
#define __cxz_range_base_cc_h__

#include "range_base.h"

namespace CNORXZ
{
    template <class Index>
    Index RangeInterface<Index>::begin() const
    {
	return Index(RangePtr(RB::mThis), 0);
    }

    template <class Index>
    Index RangeInterface<Index>::end() const
    {
	return Index(RangePtr(RB::mThis), this->size());
    }

    template <class Range>
    Sptr<Range> RangeCast<Range>::func(const RangePtr& r)
    {
	CXZ_ERROR("cast form '" << r->type().name() << "' to '"
		  << typeid(Range).name() << "' not available");
	return nullptr;
    }

    template <class Range>
    Sptr<Range> rangeCast(const RangePtr r)
    {
	if(typeid(Range) == r->type()) {
	    return std::dynamic_pointer_cast<Range>(r);
	}
	else {
	    return RangeCast<Range>::func(r);
	}
    }

}

#endif
