
#ifndef __cxz_yrange_h__
#define __cxz_yrange_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"
#include "xindex.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    // YRange!!!!
    
    // Future DynamicIndex
    //class YIndex : public XIndexBase
    class YIndex : public IndexInterface<YIndex,DType>
    {
    public:
	typedef IndexInterface<YIndex,DType> IB;
	typedef YRange RangeType;
	
	DEFAULT_MEMBERS(YIndex);
	YIndex(const RangePtr& range, SizeT pos = 0);
	YIndex(const RangePtr& range, const Vector<XIndexPtr>& is, SizeT pos = 0);

	YIndex& sync(); // remove!!!

	YIndex& operator=(SizeT pos);
	YIndex& operator++();
	YIndex& operator--();
	YIndex operator+(Int n) const;
	YIndex operator-(Int n) const;
	YIndex& operator+=(Int n);
	YIndex& operator-=(Int n);

	SizeT max() const;
	IndexId<0> id() const;
	
	DType operator*() const;
	DType operator->() const;

	Int pp(PtrId idxPtrNum);
	Int mm(PtrId idxPtrNum);

	SizeT dim() const;
	Sptr<YRange> range() const;
	UPos stepSize(const IndexId<0> id) const;

	String stringMeta() const;
	DType meta() const;
	YIndex& at(const DType& meta);

	DXpr<SizeT> ifor(const DXpr<SizeT>& xpr) const;

    private:

	Sptr<YRange> mRangePtr;
	Vector<XIndexPtr> mIs;
	Vector<SizeT> mBlockSizes; // dim() elements only!!!

    };

    class YRangeFactory : public RangeFactoryBase
    {
    public:
	YRangeFactory(const Vector<RangePtr>& rvec);
	YRangeFactory(Vector<RangePtr>&& rvec);
	YRangeFactory(const Vector<RangePtr>& rvec, const RangePtr& ref);
	YRangeFactory(Vector<RangePtr>&& rvec, const RangePtr& ref);

    private:
	YRangeFactory() = default;
	virtual void make() override final;

	Vector<RangePtr> mRVec;
	RangePtr mRef;
    };

    class YRange : public RangeInterface<YIndex,DType>
    {
    public:
	typedef RangeBase RB;
	typedef YIndex IndexType;

	friend YRangeFactory;

	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;

    private:

	YRange() = delete;
	YRange(const YRange& a) = delete;
	YRange(const Vector<RangePtr>& rvec);
	YRange(Vector<RangePtr>&& rvec);

	Vector<RangePtr> mRVec;
    };
}

#endif
