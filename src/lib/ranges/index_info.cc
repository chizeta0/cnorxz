
#include "ranges/index_info.h"
#include "ranges/range_base.h"

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


    std::vector<IndexInfo> getRootIndices(const IndexInfo& info)
    {
	std::vector<IndexInfo> out;
	out.reserve(info.dim());

	if(info.type() == IndexType::SINGLE){
	    out.push_back(info);
	}
	else {
	    for(size_t i = 0; i != info.dim(); ++i){
		auto vv = getRootIndices(*info.getPtr(i));
		out.insert(out.end(), vv.begin(), vv.end());
	    }
	}
	return out;
    }

    //inline size_t getStepSize(const IndexInfo& ii, std::intptr_t j)
    
    size_t getStepSize(const std::vector<IndexInfo>& iv, std::intptr_t j)
    {
	size_t ss = 1;
	for(auto ii = iv.end() - 1; ii != iv.begin(); --ii){
	    if(ii->getPtrNum() == j){
		return ss;
	    }
	    ss *= ii->max();
	}
	if(iv.begin()->getPtrNum() == j){
	    return ss;
	} else {
	    return 0;
	}
    }
    
} // end namespace MultiArrayTools
