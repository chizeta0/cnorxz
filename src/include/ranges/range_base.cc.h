
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

}

#endif
