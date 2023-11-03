// -*- C++ -*-
/**
   
   @file include/ranges/range_base.cc.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_range_base_cc_h__
#define __cxz_range_base_cc_h__

#include "range_base.h"
#include "dindex.h"
#include "mrange.h"

namespace CNORXZ
{

    template <class Range>
    decltype(auto) RangeInterface<Range>::begin() const
    {
	typedef typename Range::IndexType IndexType;
	return IndexType(RangePtr(RB::mThis), 0);
    }

    template <class Range>
    decltype(auto) RangeInterface<Range>::end() const
    {
	typedef typename Range::IndexType IndexType;
	return IndexType(RangePtr(RB::mThis), this->size());
    }

    template <class Range>
    DIndex RangeInterface<Range>::index(SizeT pos) const
    {
	typedef typename Range::IndexType IndexType;
	typedef typename IndexType::MetaType MetaType;
	return XIndexPtr(std::make_shared<XIndex<IndexType,MetaType>>( this->begin()+pos ));
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
