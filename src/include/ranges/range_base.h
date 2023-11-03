// -*- C++ -*-
/**
   
   @file include/range/range_base.h
   @brief RangeBase declaration

   Include basic headers containing defintions and macros
   used throughout this library

   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

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

    /**
        Abstract factory base class for creating ranges.
	Ranges can exclusively create through range factories.
	
	All ranges that are ever created are stored in a product map
	(sCreated). The object type hash and the uuid of all ranges
	the created depends on serve as keys. In the case the range does not
	depend on any other range, its own uuid is used as key. If the
	range is a result of a cast from another range, the latter's uuid is
	used as key.

	Derived factories are supposed to look for an existing range by
	calling fromCreated() and, if nullptr returned, add the new range
	to the map by calling addToCreated(). These steps are supposed to be
	implemented by overloading the purely virtual function make().

	The product is publicly accessible throught the function create().
     */
    class RangeFactoryBase
    {
    public:

	/** Default constructor */
	RangeFactoryBase() = default;
	
	/** Default destructor */
	virtual ~RangeFactoryBase() = default;

	/** Return the product */
	RangePtr create();

	static RangePtr getRegistered(const TypeInfo& info, const RangePtr& r);
	// check if range with uuid of r exists; if yes, return existing one, else add r and return it

    protected:

	/** Abstract method to create the product. */
	virtual void make() = 0;

	/** access the product map (read-only) */
	RangePtr fromCreated(const TypeInfo& info, const Vector<Uuid>& rids) const;

	/** add newly created range to the product map */
	void addToCreated(const TypeInfo& info, const Vector<Uuid>& rids, const RangePtr& r);

	RangePtr mProd; /**< shared pointer to the product */

    private:

	static Map<SizeT,Map<Vector<Uuid>,RangePtr>> sCreated; /**< the product map */
	
    };

    /**
       Abstract range base class

       A range defines a parameter space containers and operations are defined on.
       Indices can be used to iterate over that space.

       Ranges have to be created though a corresponding factory.
       @see RangeFactoryBase

       Ranges always exist within a shared pointer and cannot be copied.
     */
    class RangeBase
    {
    public:
	typedef DIndex IndexType;

	/** default destructor */
	virtual ~RangeBase() = default;
	
	/** Access sub-ranges.
	    In the case the range is multi-dimensional the num-th
	    sub-range can be accessed through this function
	    @param num size type argument
	 */
	virtual RangePtr sub(SizeT num) const;

	/** Access sub-ranges.
	    In the case the range is multi-dimensional this function returns an
	    array of all sub-ranges (empty is one-dimensional)
	 */
	virtual MArray<RangePtr> sub() const;

	/** Range size */
	virtual SizeT size() const = 0;

	/** Range dimension.
	    Returns the range's dimension, which is equivalent to
	    the number of sub-ranges. If a sub-range has non-zero dimension
	    this is NOT taken into account!
	 */
	virtual SizeT dim() const = 0;

	/** Range type */
	virtual const TypeInfo& type() const = 0;

	/** The type of the range's meta data */
	virtual const TypeInfo& metaType() const = 0;

	/** Meta data in string format at lexicographic position pos.
	    @param pos size type argument
	 */
        virtual String stringMeta(SizeT pos) const = 0;

	/** Return DIndex iterating over the range's parameter space.
	    @param pos index position, size type argument
	 */
	virtual DIndex index(SizeT pos = 0) const = 0;

	/** Create extended range.
	    Creates a new range consisting of the meta data spaces
	    of this range and the range r
	    @param r shared pointer to range used for extension
	 */
	virtual RangePtr extend(const RangePtr& r) const = 0;

	/** check if ranges are the same */
	bool operator==(const RangeBase& in) const;

	/** check if ranges are not the same */
	bool operator!=(const RangeBase& in) const;

	/** return the range's uuid */
        Uuid id() const;

	/** return a DIndex pointing to the range's first element */
	DIndex begin() const;

	/** return a DIndex pointing to the element after the range's last element */
	DIndex end() const;
	
	friend RangeFactoryBase;
	
    protected:

	/** default constructor */
	RangeBase();

	virtual Vector<Uuid> key() const = 0;
	// delete copy/move???
	Uuid mId = {0,0}; /**< The range's uuid */
	Wptr<RangeBase> mThis; /**< weak pointer to this range */
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

    RangePtr getSub(const RangePtr& r, SizeT num);
    MArray<RangePtr> getSub(const RangePtr& r);
    
}

#endif
