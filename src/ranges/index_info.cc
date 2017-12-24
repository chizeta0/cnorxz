
#include "index_info.h"

namespace MultiArrayTools
{

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
    
    size_t IndexInfo::getStepSize(size_t inum) const
    {
	return mNext[inum]->getStepSzize();
    }
    
    size_t IndexInfo::getStepSize() const
    {
	return mStepSize;
    }

    
} // end namespace MultiArrayTools
