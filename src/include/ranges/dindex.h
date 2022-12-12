
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
	typedef DType MetaType;
	typedef RangeBase RangeType;

	DEFAULT_C(DIndex);
	DIndex(const DIndex& i);
	DIndex(DIndex&& i);
	DIndex& operator=(const DIndex& i);
	DIndex& operator=(DIndex&& i);
	DIndex(const XIndexPtr& i);
	DIndex(const RangePtr& r, SizeT lexpos = 0);
	
	template <class Index, typename Meta>
	DIndex(const IndexInterface<Index,Meta>& i);

	DIndex& operator=(SizeT lexpos);
	DIndex& operator++();
	DIndex& operator--();
	DIndex operator+(Int n) const;
	DIndex operator-(Int n) const;
	DIndex& operator+=(Int n);
	DIndex& operator-=(Int n);

	SizeT lex() const;
	UPos pmax() const;
	UPos lmax() const;
	IndexId<0> id() const;
	
	DType operator*() const;

	SizeT dim() const;
	RangePtr range() const;

	UPos stepSize(const IndexId<0>& id) const;
	Vector<XIndexPtr> pack() const;
	Vector<SizeT> blockSizes() const;
	DIndex& setBlockSizes(const Vector<SizeT>& bs);

	String stringMeta() const;
	DType meta() const;
	DIndex& at(const DType& meta);

	DXpr<SizeT> ifor(const DXpr<SizeT>& xpr, std::function<SizeT(SizeT,SizeT)>&& f) const;

	const XIndexPtr& xptr() const;
	
    private:
	XIndexPtr mI;
    };
    
}

#endif
