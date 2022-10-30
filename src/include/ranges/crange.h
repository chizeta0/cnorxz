
#ifndef __cxz_crange_h__
#define __cxz_crange_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    class CIndex : public IndexInterface<CIndex,SizeT>
    {
    public:

	typedef IndexInterface<CIndex,SizeT> IB;
	typedef CRange RangeType;

	CIndex(const RangePtr& range, SizeT pos = 0);
	
	CIndex& operator=(SizeT pos);
	CIndex& operator++();
	CIndex& operator--();
	CIndex operator+(Int n) const;
	CIndex operator-(Int n) const;
	CIndex& operator+=(Int n);
	CIndex& operator-=(Int n);

	SizeT max() const;
	IndexId<0> id() const;
	
	SizeT operator*() const;
	SizeT operator->() const;
	
	Int pp(PtrId idxPtrNum);
	Int mm(PtrId idxPtrNum);

	SizeT dim() const; // = 1
	Sptr<RangeType> range() const;
	UPos stepSize(const IndexId<0>& id) const;
	
	String stringMeta() const;
	SizeT meta() const;
	CIndex& at(const SizeT& metaPos);

	template <class Xpr, class F = NoF>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;

    private:
	Sptr<RangeType> mRangePtr;
    };


    class CRangeFactory : public RangeFactoryBase
    {
    public:
	typedef CRange oType;

	CRangeFactory(SizeT size);
	CRangeFactory(SizeT size, RangePtr ref);

    protected:
	virtual void make() override;

    private:
	SizeT mSize;
	RangePtr mRef;
    };

    class CRange : public RangeInterface<CIndex,SizeT>
    {
    public:
	typedef RangeBase RB;
	typedef CIndex IndexType;
        typedef CRangeFactory FType; 
        
	friend CRangeFactory;

	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;

	SizeT get(SizeT pos) const;
	SizeT getMeta(SizeT metaPos) const;

    protected:

	CRange() = default;
	CRange(const CRange& in) = delete;
	CRange(SizeT size);

	SizeT mSize = 0; 
    };

    template <>
    struct RangeCast<CRange>
    {
	static Sptr<CRange> func(const RangePtr& r);
    };
}

#endif
