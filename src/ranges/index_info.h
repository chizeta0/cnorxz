// -*- C++ -*-

#ifndef __index_info_h__
#define __index_info_h__

#include <cstdlib>
#include <vector>

namespace MultiArrayTools
{
    class IndexInfo;
    
    class IndexInfo
    {
    public:
	IndexInfo() = delete;

	template <class IndexClass>
	IndexInfo(const IndexClass& ind, size_t stepSize = 1);
	
	const IndexInfo* getPtr(size_t inum) const;
	std::intptr_t getPtrNum() const;
	size_t dim() const;
	size_t getStepSize(size_t inum) const;
	size_t getStepSize() const;

    private:
	std::vector<IndexInfo> mNext;
	std::intptr_t mPtrNum;
	size_t mDim;
	size_t mStepSize;
    };

    template <class IndexClass>
    IndexInfo::IndexInfo(const IndexClass& ind, size_t stepSize) :
	mNext(ind.infoVec()),
	mPtrNum( reinterpret_cast<std::intptr_t>( &ind ) ),
	mDim(ind.rangePtr()->dim()),
	mStepSize(stepSize)
    {}
    
} // end namespace MultiArrayTools

#endif
