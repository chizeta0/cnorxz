
#ifndef __cxz_yindex_h__
#define __cxz_yindex_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"
#include "xindex.h"
#include "xfor/xfor.h"

namespace CNORXZ
{
    // YRange!!!!
    
    // Future DynamicIndex
    //class YIndex : public XIndexBase
    class YIndex : public IndexInterface<YIndex,DType>
    {
    public:
	typedef IndexInterface<YIndex,DType> IB;
	
    private:
	RangePtr mRange;
	Vector<XIndexPtr> mIs;
	Vector<SizeT> mBlockSizes; // dim() elements only!!!
	bool mExternalControl = false;

    public:

	DEFAULT_MEMBERS(YIndex);
	YIndex(const RangePtr& range);
	YIndex(const RangePtr& range, const Vector<XIndexPtr>& is);

	YIndex& sync();
    	YIndex& operator=(SizeT pos);
	YIndex& operator++();
	YIndex& operator--();
	int pp(PtrId idxPtrNum);
	int mm(PtrId idxPtrNum);
	size_t dim() const;
	size_t getStepSize(SizeT n) const;
	String stringMeta() const;
	DType meta() const;
	YIndex& at(const DType& meta);
	DExpr ifor(SizeT step, DExpr ex) const;
	DExpr iforh(SizeT step, DExpr ex) const;
    };

}

#endif
