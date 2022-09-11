
#ifndef __cxz_range_type_classic_def__
#define __cxz_range_type_classic_def__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xfor/for_type.h"

namespace CNORXZ
{
    class CIndex : public IndexInterface<CIndex<MetaType>,SizeT>
    {
    public:

	typedef IndexInterface<CIndex<SizeT>,SizeT> IB;
	typedef CRange RangeType;

	CIndex(const RangePtr& range);
	
	CIndex& operator=(SizeT pos);
	CIndex& operator++();
	CIndex& operator--();
	CIndex operator+(Int n) const;
	CIndex operator-(Int n) const;
	CIndex& operator+=(Int n);
	CIndex& operator-=(Int n);

	SizeT operator*() const;
	SizeT operator->() const;
	
	Int pp(PtrId idxPtrNum);
	Int mm(PtrId idxPtrNum);

	SizeT dim() const; // = 1
	Sptr<RangeType> range() const;
	SizeT getStepSize(SizeT n) const;

	String stringMeta() const;
	SizeT meta() const;
	CIndex& at(const SizeT& metaPos);

	template <class Expr>
	auto ifor(SizeT step, Expr ex) const
	    -> For<CIndex<SizeT>,Expr>;

	template <class Expr>
	auto iforh(SizeT step, Expr ex) const
	    -> For<CIndex<SizeT>,Expr,ForType::HIDDEN>;

        template <class Expr>
	auto pifor(SizeT step, Expr ex) const
	    -> PFor<CIndex<SizeT>,Expr>;
        
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
	RangePtr mRef;
    };

    class CRange : public RangeInterface<CIndex>
    {
    public:
	typedef RangeBase RB;
	typedef CIndex IndexType;
        typedef CRangeFactory FType; 
        
	friend CRangeFactory;

	virtual SizeT size() const final;
	virtual SizeT dim() const final;
	virtual String stringMeta(SizeT pos) const final;
	virtual IndexType begin() const final;
	virtual IndexType end() const final;

	SizeT get(SizeT pos) const;
	SizeT getMeta(SizeT metaPos) const;

    protected:

	CRange() = default;
	CRange(const CRange& in) = delete;
	CRange(SizeT size);

	SizeT mSize = 0; 
    };

    template <>
    Sptr<CRange> rangeCast<CRange>(const RangePtr& r);
}

#endif
