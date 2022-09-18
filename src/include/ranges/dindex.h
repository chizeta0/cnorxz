
#ifndef __cxz_dindex_h__
#define __cxz_dindex_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"

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

	DType operator*() const;
	DType operator->() const;

	Int pp(PtrId idxPtrNum);
	Int mm(PtrId idxPtrNum);

	SizeT dim() const;
	RangePtr range() const;
	SizeT getStepSize(SizeT n) const;

	String stringMeta() const;
	DType meta() const;
	DIndex& at(const DType& meta);

	//DExpr ifor(SizeT step, DExpr ex) const;
	//DExpr iforh(SizeT step, DExpr ex) const;

    private:
	XIndexPtr mI;
    };
    
}

#endif
