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

	RangePtr fromCreated(const TypeInfo& info, const Vector<Uuid>& rids) const;
	void addToCreated(const TypeInfo& info, const Vector<Uuid>& rids, const RangePtr& r);

	RangePtr mProd;

    private:
	// also add single ranges here (PtrId -> own)
	// rangeCast: PtrId -> original Range
	static Map<SizeT,Map<Vector<Uuid>,RangePtr>> sCreated;
	
    };

    Sptr<RangeFactoryBase> createRangeFactory(const char** dp);
    Sptr<RangeFactoryBase> createSingleRangeFactory(const Vector<char>*& d, int metaType, SizeT size);

    class RangeBase
    {
    public:
	typedef DIndex IndexType;
	
	virtual ~RangeBase() = default;
	
	// virtual RangePtr sub() const; // Sptr<URange<RangePtr>> ; range of subranges (TODO!!!)
	virtual RangePtr sub(SizeT num) const;
	virtual SizeT size() const = 0;
	virtual SizeT dim() const = 0;
	virtual const TypeInfo& type() const = 0;
	virtual const TypeInfo& metaType() const = 0;
        virtual String stringMeta(SizeT pos) const = 0;
	virtual DIndex index(SizeT pos = 0) const = 0;
	virtual RangePtr extend(const RangePtr& r) const = 0;
	
	bool operator==(const RangeBase& in) const;
	bool operator!=(const RangeBase& in) const;

        Uuid id() const;
	DIndex begin() const;
	DIndex end() const;
	RangePtr orig() const;
	
	friend RangeFactoryBase;
	
    protected:
	
	RangeBase();
	RangeBase(const RangePtr& rel);
	// delete copy/move???
	Uuid mId = {0,0};
	Wptr<RangeBase> mThis;
	RangePtr mRel; // used, if created from another range, to point on it
    };

    template <class Range>
    class RangeInterface : public RangeBase
    {
    public:

	typedef RangeBase RB;
	
	decltype(auto) begin() const;
	decltype(auto) end() const;
	virtual DIndex index(SizeT pos) const override final;

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

    RangePtr operator*(const RangePtr& a, const RangePtr& b); // -> Ptr to YRange

}

#endif
