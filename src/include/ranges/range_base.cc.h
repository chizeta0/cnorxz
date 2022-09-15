
#ifndef __cxz_range_base_cc_h__
#define __cxz_range_base_cc_h__

#include "range_base.h"

namespace CNORXZ
{
    template <class Index, typename Meta>
    Index RangeInterface<Index,Meta>::begin() const
    {
	return Index(RangePtr(RB::mThis), 0);
    }

    template <class Index, typename Meta>
    Index RangeInterface<Index,Meta>::end() const
    {
	return Index(RangePtr(RB::mThis), this->size());
    }

    template <class Index, typename Meta>
    XIndexPtr RangeInterface<Index,Meta>::index(SizeT pos) const
    {
	return std::make_shared<XIndex<Index,Meta>>( this->begin()+pos );
    }
	
    template <class Range>
    Sptr<Range> RangeCast<Range>::func(const RangePtr& r)
    {
	CXZ_ERROR("cast from '" << r->type().name() << "' to '"
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
