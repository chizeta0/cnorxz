// -*- C++ -*-
#include "index_base.h"

namespace MultiArrayTools
{
    /*****************
     *   IndexBase   *
     *****************/

    IndexBase::IndexBase(const std::shared_ptr<RangeBase>& range,
			 size_t pos) : mRangePtr(range),
				       mPos(pos) {}
    
    bool IndexBase::operator==(const IndexBase& in) const
    {
	return in.mPos == mPos and in.mRangePtr.get() == mRangePtr.get();
    }
    
    bool IndexBase::operator!=(const IndexBase& in) const
    {
	return in.mPos != mPos or in.mRangePtr.get() != mRangePtr.get();
    }
    
    size_t IndexBase::pos() const
    {
	return mPos;
    }

    size_t IndexBase::max() const
    {
	return mRangePtr->size();
    }

    operator IndexBase::size_t() const
    {
	return pos();
    }
    
    /**********************
     *   IndexInterface   *	     
     **********************/

    template <typename MetaType>
    IndexInterface::IndexInterface(const RangeBase& range, size_t pos) :
	IndexBase(range, pos) {}
    
}
