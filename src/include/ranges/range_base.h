// -*- C++ -*-

#ifndef __cxz_range_base_h__
#define __cxz_range_base_h__

#include "base/base.h"
#include "base/base.cc.h"
#include "memory/memory.h"
#include "memory/memory.cc.h"

#ifdef HAVE_CEREAL
#include <cereal/access.hpp>
#endif

namespace CNORXZ
{

    class RangeFactoryBase
    {
    public:

	RangeFactoryBase() = default;
	virtual ~RangeFactoryBase() = default;
	
	RangePtr create();
	
	static RangePtr getRegistered(const TypeInfo& info, const RangePtr& r);
	// check if range with uuid of r exists; if yes, return existing one, else add r and return it

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
	
	//virtual Vector<Uuid> key2() const { return Vector<Uuid>(); }

	friend RangeFactoryBase;
	
    protected:
	
	RangeBase();
	RangeBase(const RangePtr& rel);

	
	virtual Vector<Uuid> key() const = 0;
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


    struct RangePack
    {
	Vector<RangePtr> mRs;
	operator RangePtr() const; // -> Ptr to YRange
    };

    RangePack operator*(const RangePtr& a, const RangePtr& b);
    RangePack operator*(const RangePtr& a, const RangePack& b);
    RangePack operator*(const RangePack& a, const RangePtr& b);
    RangePack operator*(const RangePack& a, const RangePack& b);
}

#endif
