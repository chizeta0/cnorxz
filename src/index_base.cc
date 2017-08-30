// -*- C++ -*-
#include "index_base.h"

namespace MultiArrayTools
{
    /*****************
     *   IndexBase   *
     *****************/

    IndexBase::IndexBase(const std::shared_ptr<RangeBase>& range,
			 size_t pos) : mRangePtr(range),
				       mPos(pos)
    {
	mId = indexId();
    }
    
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
    /*
    bool IndexBase::locked() const
    {
	return mLocked;
    }
    
    IndexBase& IndexBase::lock(std::shared_ptr<const IndexBase>& idx)
    {
	mLocked = (idx.get() == this);
	return *this;
    }
    */
    
    IndexBase::operator size_t() const
    {
	return pos();
    }
    
    /**********************
     *   IndexInterface   *	     
     **********************/

    template <typename MetaType>
    IndexInterface<MetaType>::IndexInterface(const std::shared_ptr<RangeBase>& rangePtr, size_t pos) :
	IndexBase(rangePtr, pos) {}
    
}
