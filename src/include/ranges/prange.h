
#ifndef __cxz_prange_h__
#define __cxz_prange_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{

    template <class Index>
    class PIndex : public IndexInterface<Index,typename Index::MetaType>
    {
    public:

	typedef public IndexInterface<Index,typename Index::MetaType> IB;
	typedef PRange<typename Index::RangeType> RangeType;
	typedef typename Index::MetaType MetaType;

	PIndex(const RangePtr& range, SizeT pos = 0);
	
	PIndex& operator=(SizeT lexpos);
	PIndex& operator++();
	PIndex& operator--();
	PIndex operator+(Int n) const;
	PIndex operator-(Int n) const;
	PIndex& operator+=(Int n);
	PIndex& operator-=(Int n);

	SizeT lex() const;
	UPos pmax() const;
	UPos lmax() const;
	IndexId<0> id() const;
	
	const MetaT& operator*() const;
	
	SizeT dim() const;
	Sptr<RangeType> range() const;

	template <SizeT I>
	UPos stepSize(const IndexId<I>& id) const;

	String stringMeta() const;
	const MetaT& meta() const;
	PIndex& at(const MetaT& metaPos);
	decltype(auto) xpr(const Sptr<PIndex<Index>>& _this) const;

	template <class I>
	decltype(auto) format(const Sptr<I>& ind) const;

	template <class I>
	decltype(auto) slice(const Sptr<I>& ind) const;

	template <class Xpr, class F>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;

	PIndex& operator()();
	PIndex& operator()(const Sptr<Index>& i);
	const Sptr<Index>& orig() const;
	
    private:
	Sptr<RangeType> mRangePtr;
	Sptr<Index> mOrig;

	void mkPos();
    };

    template <class Range>
    class PRangeFactory : public RangeFactoryBase
    {
    public:
	PRangeFactory(const Sptr<Range>& range, const Vector<SizeT>& _parts);
	
    private:
	PRangeFactory() = default;
	virtual void make() override final;

	RangePtr mRef;
    };
    
    template <class Range>
    class PRange : public RangeInterface<PRange<Range>>
    {
    public:
	typedef RangeBase RB;
	typedef PIndex<typename Range::IndexType> IndexType;
	
	friend PRangeFactory<Range>;

	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	virtual RangePtr extend(const RangePtr& r) const override final;

	RangePtr orig() const;
	const Vector<SizeT>& parts() const;
	RangePtr derive() const;
	
    private:

	PRange() = delete;
	PRange(const PRange& in) = delete;
	PRange(const Sptr<Range>& range, const Vector<SizeT>& _parts);

	Sptr<Range> mRange;
	Vector<SizeT> mParts;
    };
    
} // namespace CNORXZ

#endif
