
#ifndef __cxz_yindex_h__
#define __cxz_yindex_h__

#include "base/base.h"
#include "ranges/range_base.h"
#include "ranges/index_base.h"
#include "xfor/xfor.h"

namespace CNORXZ
{
    // Future DynamicIndex
    //class YIndex : public IndexInterface<YIndex,DType>
    class YIndex : public XIndexBase
    {
    public:
	typedef IndexInterface<YIndex,DType> IB;
	
    private:
	RangePtr mRange;
	vector<XIndexPtr> mIs;
	std::vector<size_t> mBlockSizes; // dim() elements only!!!
	bool mExternalControl = false;

    public:

	DEFAULT_MEMBERS(YIndex);
	YIndex(const RangePtr& range);
	YIndex(const RangePtr& range, const std::vector<XIndexPtr>& is);

    	YIndex& operator=(size_t pos);
	YIndex& operator++();
	YIndex& operator--();
	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);
	size_t dim() const;
	size_t getStepSize(size_t n) const;
	std::string stringMeta() const;
	DType meta() const;
	YIndex& at(const DType& meta);
	DynamicExpression ifor(size_t step, DynamicExpression ex) const;
	DynamicExpression iforh(size_t step, DynamicExpression ex) const;
    };

    typedef std::shared_ptr<YIndex> YIndexPtr;
}

#endif
