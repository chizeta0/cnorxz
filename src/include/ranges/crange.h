// -*- C++ -*-
/**
   
   @file include/ranges/crange.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_crange_h__
#define __cxz_crange_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    /** ****
	specific index for CRange
     */
    class CIndex : public IndexInterface<CIndex,SizeT>
    {
    public:

	typedef IndexInterface<CIndex,SizeT> IB;
	typedef CRange RangeType;
	typedef SizeT MetaType;

	INDEX_RANDOM_ACCESS_ITERATOR_DEFS(MetaType);
	DEFAULT_MEMBERS(CIndex); /**< default constructors and assignments */

	/** constrcut index from range and position
	    @param range Range to iterate over
	    @param pos lexicographic position
	 */
	CIndex(const RangePtr& range, SizeT pos = 0);

	/** @copydoc IndexInterface::operator=(SizeT) */
	CIndex& operator=(SizeT lexpos);

	/** @copydoc IndexInterface::operator++() */
	CIndex& operator++();

	/** @copydoc IndexInterface::operator--() */
	CIndex& operator--();

	/** @copydoc IndexInterface::operator+() */
	CIndex operator+(Int n) const;

	/** @copydoc IndexInterface::operator-() */
	CIndex operator-(Int n) const;

	/** @copydoc IndexInterface::operator-(CIndex) */
	SizeT operator-(const CIndex& i) const;

	/** @copydoc IndexInterface::operator+=() */
	CIndex& operator+=(Int n);

	/** @copydoc IndexInterface::operator-=() */
	CIndex& operator-=(Int n);

	/** @copydoc IndexInterface::lex() */
	SizeT lex() const;

	/** @copydoc IndexInterface::pmax() */
	UPos pmax() const;

	/** @copydoc IndexInterface::lmax() */
	UPos lmax() const;

	/** @copydoc IndexInterface::id() */
	IndexId<0> id() const;
	
	/** @copydoc IndexInterface::operator*() */
	SizeT operator*() const;
	
	/** @copydoc IndexInterface::dim() */
	SizeT dim() const; // = 1

	/** @copydoc IndexInterface::range() */
	Sptr<RangeType> range() const;

	/** @copydoc IndexInterface::stepSize() */
	template <SizeT I>
	decltype(auto) stepSize(const IndexId<I>& id) const;
	
	/** @copydoc IndexInterface::stringMeta() */
	String stringMeta() const;

	/** @copydoc IndexInterface::meta() */
	SizeT meta() const;

	/** @copydoc IndexInterface::at() */
	CIndex& at(const SizeT& metaPos);

	/** @copydoc IndexInterface::xpr() */
	COpRoot<SizeT,CIndex> xpr(const Sptr<CIndex>& _this) const;

	/** @copydoc IndexInterface::prange() */
	RangePtr prange(const CIndex& last) const;

	/** @copydoc IndexInterface::deepFormat() */
	SizeT deepFormat() const;

	/** @copydoc IndexInterface::ifor() */
	template <class Xpr, class F = NoF>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;

    private:
	Sptr<RangeType> mRangePtr;
    };

    /** make index pack of a CIndex and another index
	@tparam type of the second index
	@param a pointer to CIndex
	@param b pointer to another index
     */
    template <class I>
    decltype(auto) operator*(const Sptr<CIndex>& a, const Sptr<I>& b);

    /** ****
	specific factory for CRange
     */
    class CRangeFactory : public RangeFactoryBase
    {
    public:
	typedef CRange oType;

	/** construct and setup factory
	    @param size size of the range to be constructed
	 */
	CRangeFactory(SizeT size);

	/** construct and setup factory
	    @param size size of the range to be constructed
	    @param ref range the range to be constructed is related to
	 */
	CRangeFactory(SizeT size, RangePtr ref);

    protected:
	virtual void make() override;

    private:
	SizeT mSize;
	RangePtr mRef;
    };

    /** ****
	Classic Range
	The parameter space is given by a
	set of positve integer numbers running
	form 0 to size-1
     */
    class CRange : public RangeInterface<CRange>
    {
    public:
	typedef RangeBase RB;
	typedef CIndex IndexType;
	typedef SizeT MetaType;
        typedef CRangeFactory FType; 
        
	friend CRangeFactory;

	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	virtual RangePtr extend(const RangePtr& r) const override final;

	/** return meta data at given position
	    @param pos position, size type
	 */
	SizeT get(SizeT pos) const;

	/** return position for given meta data
	    @param metaPos meta data, size type
	 */
	SizeT getMeta(SizeT metaPos) const;

    protected:

	/** default constructor */
	CRange() = default;

	CRange(const CRange& in) = delete;

	/** create range of given size
	    @param size, input size, size type
	 */
	CRange(SizeT size);

	virtual Vector<Uuid> key() const override final;
	
	SizeT mSize = 0; /**< range size */

	SERIALIZATION_FUNCTIONS_NOPUB;
    };

    /** ***
	Specialize RangeCast for casts to CRange
	@see RangeCast
     */    
    template <>
    struct RangeCast<CRange>
    {
	/** cast the range */
	static Sptr<CRange> func(const RangePtr& r);
    };
}

#endif
