// -*- C++ -*-

#ifndef __cxz_srange_h__
#define __cxz_srange_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    template <typename MetaT, SizeT S>
    class SIndex : public IndexInterface<SIndex<MetaT,S>,MetaT>
    {
    public:
	typedef IndexInterface<SIndex<MetaT,S>,MetaT> IB;
	typedef SRange<MetaT,S> RangeType;
	typedef MetaT MetaType;

	INDEX_RANDOM_ACCESS_ITERATOR_DEFS(MetaType);
	DEFAULT_MEMBERS(SIndex);
	SIndex(const RangePtr& range, SizeT pos = 0);

	SIndex& operator=(SizeT lexpos);
	SIndex& operator++();
	SIndex& operator--();
	SIndex operator+(Int n) const;
	SIndex operator-(Int n) const;
	SIndex& operator+=(Int n);
	SIndex& operator-=(Int n);

	SizeT lex() const;
	SPos<S> pmax() const;
	SPos<S> lmax() const;
	IndexId<0> id() const;
	
	const MetaT& operator*() const;
	
	SizeT dim() const; // = 1
	Sptr<RangeType> range() const;

	template <SizeT I>
	UPos stepSize(const IndexId<I>& id) const;

	String stringMeta() const;
	const MetaT& meta() const;
	SIndex& at(const MetaT& metaPos);
	decltype(auto) xpr(const Sptr<SIndex<MetaType,S>>& _this) const;

	template <class Index>
	decltype(auto) reformat(const Sptr<Index>& ind) const;

	template <class Index>
	decltype(auto) slice(const Sptr<Index>& ind) const;

	template <class Xpr, class F>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;

    private:
	Sptr<RangeType> mRangePtr;
	const MetaT* mMetaPtr;
    };

    template <typename MetaType, SizeT S, class I1>
    decltype(auto) operator*(const Sptr<SIndex<MetaType,S>>& a, const Sptr<I1>& b);

    template <typename MetaType, SizeT S>
    class SRangeFactory : public RangeFactoryBase
    {
    public:
	SRangeFactory(const Arr<MetaType,S>& space);
        SRangeFactory(Arr<MetaType,S>&& space);
	SRangeFactory(const Arr<MetaType,S>& space, const RangePtr& ref);
        SRangeFactory(Arr<MetaType,S>&& space, const RangePtr& ref);
	
    private:
	SRangeFactory() = default;
	virtual void make() override final;

	Arr<MetaType,S> mSpace;
	RangePtr mRef;
    };

    template <typename MetaType, SizeT S>
    class SRange : public RangeInterface<SRange<MetaType,S>>
    {
    public:
	typedef RangeBase RB;
	typedef SIndex<MetaType,S> IndexType;

	friend SRangeFactory<MetaType,S>;

	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	virtual RangePtr extend(const RangePtr& r) const override final;

	const MetaType& get(SizeT pos) const;
	const MetaType* get() const;
	SizeT getMeta(const MetaType& metaPos) const;

    private:

	SRange() = default;
	SRange(const SRange& in) = delete;
	SRange(const Arr<MetaType,S>& space);
	SRange(Arr<MetaType,S>&& space);

	Arr<MetaType,S> mSpace;

	virtual Vector<Uuid> key() const override final;

	SERIALIZATION_FUNCTIONS_NOPUB;
    };

    template <typename MetaType, SizeT S>
    struct RangeCast<SRange<MetaType,S>>
    {
	static Sptr<SRange<MetaType,S>> func(const RangePtr& r);
    };
    
}

#endif