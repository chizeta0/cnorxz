// -*- C++ -*-

#ifndef __cxz_urange_h__
#define __cxz_urange_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xfor/for_type.h"

namespace CNORXZ
{
    
    template <typename MetaType>
    class UIndex : public IndexInterface<UIndex<MetaType>,MetaType>
    {
    public:

	typedef IndexInterface<UIndex<MetaType>,MetaType> IB;
	typedef URange<MetaType> RangeType;

	UIndex(const RangePtr& range, SizeT pos = 0);
	
	UIndex& operator=(SizeT pos);
	UIndex& operator++();
	UIndex& operator--();
	UIndex operator+(Int n) const;
	UIndex operator-(Int n) const;
	UIndex& operator+=(Int n);
	UIndex& operator-=(Int n);

	const MetaType& operator*() const;
	const MetaType* operator->() const;
	
	Int pp(PtrId idxPtrNum);
	Int mm(PtrId idxPtrNum);

	SizeT dim() const; // = 1
	Sptr<RangeType> range() const;
	SizeT getStepSize(PtrId iptr) const;
	Int getOffset(PtrId iptr) const;

	String stringMeta() const;
	const MetaType& meta() const;
	UIndex& at(const MetaType& metaPos);
	/*
	template <class Expr>
	auto ifor(SizeT step, Expr ex) const
	    -> For<UIndex<MetaType>,Expr>;

	template <class Expr>
	auto iforh(SizeT step, Expr ex) const
	    -> For<UIndex<MetaType>,Expr,ForType::HIDDEN>;

        template <class Expr>
	auto pifor(SizeT step, Expr ex) const
	    -> PFor<UIndex<MetaType>,Expr>;
	*/
    private:
	Sptr<RangeType> mRangePtr;
	const MetaType* mMetaPtr;
    };

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
    class URange : public RangeInterface<UIndex<MetaType>,MetaType>
    {
    public:
	typedef RangeBase RB;
	typedef UIndex<MetaType> IndexType;
	
	friend URangeFactory<MetaType>;

	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual IndexType begin() const override final;
	virtual IndexType end() const override final;

	const MetaType& get(SizeT pos) const;
	SizeT getMeta(const MetaType& metaPos) const;
	
    private:

	URange() = delete;
	URange(const URange& in) = delete;
	URange(const Vector<MetaType>& space);
        URange(Vector<MetaType>&& space);

	Vector<MetaType> mSpace;
    };

    template <typename MetaType>
    struct RangeCast<URange<MetaType>>
    {
	static Sptr<URange<MetaType>> func(const RangePtr& r);
    };
}

#endif
