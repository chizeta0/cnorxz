// -*- C++ -*-

#ifndef __index_info_h__
#define __index_info_h__

#include <cstdlib>
#include <cstdint>
#include <vector>
#include <memory>
#include "vindex_base.h"
#include "index_type.h"

namespace MultiArrayTools
{
    class IndexInfo;
    
    class IndexInfo
    {
    public:

	IndexInfo(IndexInfo&& in) = default;
	IndexInfo& operator=(IndexInfo&& in) = default;
	IndexInfo(const IndexInfo& in) = default;
	IndexInfo& operator=(const IndexInfo& in) = default;
	
	template <class IndexClass>
	IndexInfo(const IndexClass& ind, size_t stepSize = 1);

	template <class IndexClass>
	IndexInfo& reassign(const IndexClass& ind, size_t stepSize = 1);
	
	bool operator==(const IndexInfo& in) const;
	bool operator!=(const IndexInfo& in) const;

	bool operator<=(const IndexInfo& in) const;
	bool operator<(const IndexInfo& in) const;
	bool operator>(const IndexInfo& in) const;
	bool operator>=(const IndexInfo& in) const;

	
	const IndexInfo* getPtr(size_t inum) const;
	std::intptr_t getPtrNum() const;
	size_t dim() const;
	size_t max() const;
	size_t getStepSize(size_t inum) const;
	size_t getStepSize() const;
	IndexType type() const;
	
    private:

	IndexInfo() = default;
	
	std::vector<IndexInfo> mNext;
	std::intptr_t mPtrNum;
	size_t mDim;
	size_t mMax;
	size_t mStepSize;
	IndexType mType;
    };

    template <class IndexClass>
    IndexInfo::IndexInfo(const IndexClass& ind, size_t stepSize) :
	mNext(ind.infoVec()),
	mPtrNum( reinterpret_cast<std::intptr_t>( &ind ) ),
	mDim(ind.vrange()->dim()),
	mMax(ind.max()),
	mStepSize(stepSize),
	mType(ind.type())
    {}

    template <class IndexClass>
    IndexInfo& IndexInfo::reassign(const IndexClass& ind, size_t stepSize)
    {
	IndexInfo ii(ind, stepSize);
	(*this) = std::move(ii);
	return *this;
    }

    std::vector<IndexInfo> getRootIndices(const IndexInfo& info);

    size_t getStepSize(const std::vector<IndexInfo>& iv, const IndexInfo& j);

    size_t getStepSize(const std::vector<IndexInfo>& iv, std::intptr_t j);
} // end namespace MultiArrayTools

#endif
