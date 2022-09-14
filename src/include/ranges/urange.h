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

	UIndex(const RangePtr& range);
	
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
	SizeT getStepSize(SizeT n) const;

	String stringMeta() const;
	const MetaType& meta() const;
	UIndex& at(const MetaType& metaPos);

	template <class Expr>
	auto ifor(SizeT step, Expr ex) const
	    -> For<UIndex<MetaType>,Expr>;

	template <class Expr>
	auto iforh(SizeT step, Expr ex) const
	    -> For<UIndex<MetaType>,Expr,ForType::HIDDEN>;

        template <class Expr>
	auto pifor(SizeT step, Expr ex) const
	    -> PFor<UIndex<MetaType>,Expr>;
        
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
	
    protected:
	URangeFactory() = default;
	virtual void make() override;

    private:
	Vector<MetaType> mSpace;
	RangePtr mRef = nullptr;
    };

    template <typename MetaType>
    class URange : public RangeInterface<UIndex<MetaType>>
    {
    public:
	typedef RangeBase RB;
	typedef UIndex<MetaType> IndexType;
	typedef URangeFactory<MetaType> FType; 
	
	friend URangeFactory<MetaType>;

	virtual SizeT size() const final;
	virtual SizeT dim() const final;
	virtual String stringMeta(SizeT pos) const final;
	virtual IndexType begin() const final;
	virtual IndexType end() const final;

	const MetaType& get(SizeT pos) const;
	SizeT getMeta(const MetaType& metaPos) const;
	
		
    protected:

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
