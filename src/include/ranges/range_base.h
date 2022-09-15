// -*- C++ -*-

#ifndef __cxz_range_base_h__
#define __cxz_range_base_h__

#include "base/base.h"
#include "base/base.cc.h"
#include "memory/memory.h"
#include "memory/memory.cc.h"

namespace CNORXZ
{

    class RangeFactoryBase
    {
    public:

	RangeFactoryBase() = default;
	virtual ~RangeFactoryBase() = default;

	RangePtr create();
	
    protected:

	virtual void make() = 0;

	RangePtr fromCreated(const TypeInfo& info, const Vector<PtrId>& rids) const;
	void addToCreated(const TypeInfo& info, const Vector<PtrId>& rids, const RangePtr& r);

	RangePtr mProd;

    private:
	// also add single ranges here (PtrId -> own)
	// rangeCast: PtrId -> original Range
	static Map<SizeT,Map<Vector<PtrId>,RangePtr>> sCreated;
	
    };

    Sptr<RangeFactoryBase> createRangeFactory(const char** dp);
    Sptr<RangeFactoryBase> createSingleRangeFactory(const Vector<char>*& d, int metaType, SizeT size);

    class RangeBase
    {
    public:
	
	virtual ~RangeBase() = default;
	    
	virtual SizeT size() const = 0;
	virtual SizeT dim() const = 0;
	virtual const TypeInfo& type() const = 0;
	virtual const TypeInfo& metaType() const = 0;
        virtual String stringMeta(SizeT pos) const = 0;
	virtual XIndexPtr index(SizeT pos = 0) const = 0;
	
	bool operator==(const RangeBase& in) const;
	bool operator!=(const RangeBase& in) const;

        PtrId id() const;
	XIndexPtr begin() const;
	XIndexPtr end() const;
	RangePtr orig() const;
	
	friend RangeFactoryBase;
	
    protected:
	
	RangeBase() = default;
	RangeBase(const RangePtr& rel);
	// delete copy/move???
	Wptr<RangeBase> mThis;
	RangePtr mRel; // used, if created from another range, to point on it
    };

    template <class Index, typename Meta>
    class RangeInterface : public RangeBase
    {
    public:

	typedef Index IndexType;
	typedef RangeBase RB;
	
	Index begin() const;
	Index end() const;
	virtual XIndexPtr index(SizeT pos) const override final;

    protected:
	RangeInterface() = default;
    };

    template <class Range>
    struct RangeCast
    {
	static Sptr<Range> func(const RangePtr& r);
    };
    
    template <class Range>
    Sptr<Range> rangeCast(const RangePtr r);

    RangePtr operator*(const RangePtr& a, const RangePtr& b); // -> Ptr to MultiRange
}

#endif
