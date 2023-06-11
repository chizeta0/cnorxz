// -*- C++ -*-

#ifndef __cxz_urange_h__
#define __cxz_urange_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

#include <iterator>

namespace CNORXZ
{
    
    template <typename MetaT>
    class UIndex : public IndexInterface<UIndex<MetaT>,MetaT>
    {
    public:

	typedef IndexInterface<UIndex<MetaT>,MetaT> IB;
	typedef URange<MetaT> RangeType;
	typedef MetaT MetaType;

	INDEX_RANDOM_ACCESS_ITERATOR_DEFS(MetaType);
	DEFAULT_MEMBERS(UIndex);
	UIndex(const RangePtr& range, SizeT pos = 0);

	void swap(UIndex& i) {}; // !!!
	
	UIndex& operator=(SizeT lexpos);
	UIndex& operator++();
	UIndex& operator--();
	UIndex operator+(Int n) const;
	UIndex operator-(Int n) const;
	SizeT operator-(const UIndex& i) const { return lex() - i.lex(); } // !!!
	UIndex& operator+=(Int n);
	UIndex& operator-=(Int n);

	SizeT lex() const;
	UPos pmax() const;
	UPos lmax() const;
	IndexId<0> id() const;
	
	const MetaT& operator*() const;
	
	SizeT dim() const; // = 1
	Sptr<RangeType> range() const;

	template <SizeT I>
	UPos stepSize(const IndexId<I>& id) const;

	String stringMeta() const;
	const MetaT& meta() const;
	UIndex& at(const MetaT& metaPos);
	decltype(auto) xpr(const Sptr<UIndex<MetaType>>& _this) const;

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

    template <typename MetaT>
    void swap(UIndex<MetaT>& a, UIndex<MetaT>& b) { a.swap(b); } 
    
    template <typename MetaType, class I1>
    decltype(auto) operator*(const Sptr<UIndex<MetaType>>& a, const Sptr<I1>& b);
    
    template <typename MetaType>
    class URangeFactory : public RangeFactoryBase
    {
    public:
	URangeFactory(const Vector<MetaType>& space);
        URangeFactory(Vector<MetaType>&& space);
	URangeFactory(const Vector<MetaType>& space, const RangePtr& ref);
        URangeFactory(Vector<MetaType>&& space, const RangePtr& ref);
	
    private:
	URangeFactory() = default;
	virtual void make() override final;

	Vector<MetaType> mSpace;
	RangePtr mRef;
    };

    template <typename MetaType>
    class URange : public RangeInterface<URange<MetaType>>
    {
    public:
	typedef RangeBase RB;
	typedef UIndex<MetaType> IndexType;
	
	friend URangeFactory<MetaType>;

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

	URange() = default;
	URange(const URange& in) = delete;
	URange(const Vector<MetaType>& space);
        URange(Vector<MetaType>&& space);

	Vector<MetaType> mSpace;

	virtual Vector<Uuid> key() const override final;
	
	SERIALIZATION_FUNCTIONS_NOPUB;
    };

    template <typename MetaType>
    struct RangeCast<URange<MetaType>>
    {
	static Sptr<URange<MetaType>> func(const RangePtr& r);
    };
}

#endif
