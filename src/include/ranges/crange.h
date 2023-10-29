
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
	typedef SizeT MetaType;

	INDEX_RANDOM_ACCESS_ITERATOR_DEFS(MetaType);
	DEFAULT_MEMBERS(CIndex);
	CIndex(const RangePtr& range, SizeT pos = 0);
	
	CIndex& operator=(SizeT lexpos);
	CIndex& operator++();
	CIndex& operator--();
	CIndex operator+(Int n) const;
	CIndex operator-(Int n) const;
	SizeT operator-(const CIndex& i) const;
	CIndex& operator+=(Int n);
	CIndex& operator-=(Int n);

	SizeT lex() const;
	UPos pmax() const;
	UPos lmax() const;
	IndexId<0> id() const;
	
	SizeT operator*() const;
	
	SizeT dim() const; // = 1
	Sptr<RangeType> range() const;

	template <SizeT I>
	decltype(auto) stepSize(const IndexId<I>& id) const;
	
	String stringMeta() const;
	SizeT meta() const;
	CIndex& at(const SizeT& metaPos);
	COpRoot<SizeT,CIndex> xpr(const Sptr<CIndex>& _this) const;

	RangePtr prange(const CIndex& last) const;

	SizeT deepFormat() const;
	/*
	template <class Index>
	decltype(auto) formatFrom(const Index& ind) const;

	template <class Index>
	decltype(auto) slice(const Sptr<Index>& ind) const;
	*/
	template <class Xpr, class F = NoF>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;

    private:
	Sptr<RangeType> mRangePtr;
    };

    template <class I>
    decltype(auto) operator*(const Sptr<CIndex>& a, const Sptr<I>& b);

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

    class CRange : public RangeInterface<CRange>
    {
    public:
	typedef RangeBase RB;
	typedef CIndex IndexType;
	typedef SizeT MetaType;
        typedef CRangeFactory FType; 
        
	friend CRangeFactory;

	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	virtual RangePtr extend(const RangePtr& r) const override final;

	SizeT get(SizeT pos) const;
	SizeT getMeta(SizeT metaPos) const;

    protected:

	CRange() = default;
	CRange(const CRange& in) = delete;
	CRange(SizeT size);

	virtual Vector<Uuid> key() const override final;
	
	SizeT mSize = 0; 

	SERIALIZATION_FUNCTIONS_NOPUB;
    };

    template <>
    struct RangeCast<CRange>
    {
	static Sptr<CRange> func(const RangePtr& r);
    };
}

#endif
