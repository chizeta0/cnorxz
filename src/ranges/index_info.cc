
#include "index_info.h"
#include "range_base.h"

namespace MultiArrayTools
{
    
    bool IndexInfo::operator==(const IndexInfo& in) const
    {
	return mPtrNum == in.mPtrNum;
    }
    
    bool IndexInfo::operator!=(const IndexInfo& in) const
    {
	return mPtrNum != in.mPtrNum;
    }

    bool IndexInfo::operator<=(const IndexInfo& in) const
    {
	return mPtrNum <= in.mPtrNum;
    }

    bool IndexInfo::operator<(const IndexInfo& in) const
    {
	return mPtrNum < in.mPtrNum;
    }

    bool IndexInfo::operator>(const IndexInfo& in) const
    {
	return mPtrNum > in.mPtrNum;
    }

    bool IndexInfo::operator>=(const IndexInfo& in) const
    {
	return mPtrNum >= in.mPtrNum;
    }

    
    const IndexInfo* IndexInfo::getPtr(size_t inum) const
    {
	return &mNext[inum];
    }
    
    std::intptr_t IndexInfo::getPtrNum() const
    {
	return mPtrNum;
    }
    
    size_t IndexInfo::dim() const
    {
	return mDim;
    }

    size_t IndexInfo::max() const
    {
	return mMax;
    }
    
    size_t IndexInfo::getStepSize(size_t inum) const
    {
	return mNext[inum].getStepSize();
    }
    
    size_t IndexInfo::getStepSize() const
    {
	return mStepSize;
    }

    IndexType IndexInfo::type() const
    {
	return mType;
    }
    
} // end namespace MultiArrayTools
