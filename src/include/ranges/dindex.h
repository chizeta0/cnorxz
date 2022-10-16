
#ifndef __cxz_dindex_h__
#define __cxz_dindex_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    class DIndex : public IndexInterface<DIndex,DType>
    {
    public:
	typedef IndexInterface<DIndex,DType> IB;

	DEFAULT_C(DIndex);
	DIndex(const DIndex& i);
	DIndex(DIndex&& i);
	DIndex& operator=(const DIndex& i);
	DIndex& operator=(DIndex&& i);
	DIndex(const XIndexPtr& i);
	
	template <class Index, typename Meta>
	DIndex(const IndexInterface<Index,Meta>& i);

	DIndex& operator=(SizeT pos);
	DIndex& operator++();
	DIndex& operator--();
	DIndex operator+(Int n) const;
	DIndex operator-(Int n) const;
	DIndex& operator+=(Int n);
	DIndex& operator-=(Int n);

	SizeT max() const;
	IndexId<0> id() const;
	
	DType operator*() const;
	DType operator->() const;

	Int pp(PtrId idxPtrNum);
	Int mm(PtrId idxPtrNum);

	SizeT dim() const;
	RangePtr range() const;

	UPos stepSize(const IndexId<0>& id) const;

	String stringMeta() const;
	DType meta() const;
	DIndex& at(const DType& meta);

	DXpr ifor(const UPos& step, const DXpr& xpr) const;

    private:
	XIndexPtr mI;
    };
    
}

#endif
