// -*- C++ -*-
/**
   
   @file include/ranges/prange.h
   @brief PRange, PRangeFactory and PIndex declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_prange_h__
#define __cxz_prange_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    /** ****
	Index specific for PRange.
	@tparam IndexT Full index type.
     */
    template <class IndexT>
    class PIndex : public IndexInterface<PIndex<IndexT>,typename IndexT::MetaType>
    {
    public:

	typedef IndexInterface<PIndex<IndexT>,typename IndexT::MetaType> IB;
	typedef PRange<typename IndexT::RangeType> RangeType;
	typedef typename IndexT::MetaType MetaType;

	/** Constructor.
	    @param range Range to define index on.
	    @param pos Initial lexicographic position (default = 0).
	 */
	PIndex(const RangePtr& range, SizeT pos = 0);
	
	/** @copydoc IndexInterface::operator=(SizeT) */
	PIndex& operator=(SizeT lexpos);

	/** @copydoc IndexInterface::operator++() */
	PIndex& operator++();

	/** @copydoc IndexInterface::operator--() */
	PIndex& operator--();

	/** @copydoc IndexInterface::operator+() */
	PIndex operator+(Int n) const;

	/** @copydoc IndexInterface::operator-() */
	PIndex operator-(Int n) const;

	/** @copydoc IndexInterface::operator-(PIndex) */
	SizeT operator-(const PIndex& i) const;

	/** @copydoc IndexInterface::operator+=() */
	PIndex& operator+=(Int n);

	/** @copydoc IndexInterface::operator-=() */
	PIndex& operator-=(Int n);

	/** @copydoc IndexInterface::lex() */
	SizeT lex() const;

	/** @copydoc IndexInterface::pmax() */
	UPos pmax() const;

	/** @copydoc IndexInterface::lmax() */
	UPos lmax() const;

	/** @copydoc IndexInterface::id() */
	IndexId<0> id() const;
	
	/** @copydoc IndexInterface::operator*() */
	decltype(auto) operator*() const;
	
	/** @copydoc IndexInterface::dim() */
	SizeT dim() const;

	/** @copydoc IndexInterface::range() */
	Sptr<RangeType> range() const;

	/** @copydoc IndexInterface::stepSize() */
	template <SizeT I>
	UPos stepSize(const IndexId<I>& id) const;

	/** @copydoc IndexInterface::stringMeta() */
	String stringMeta() const;

	/** @copydoc IndexInterface::meta() */
	decltype(auto) meta() const;

	/** @copydoc IndexInterface::at() */
	PIndex& at(const MetaType& metaPos);

	/** @copydoc IndexInterface::prange() */
	RangePtr prange(const PIndex<IndexT>& last) const;

	/** @copydoc IndexInterface::deepFormat() */
	decltype(auto) deepFormat() const;

	/** @copydoc IndexInterface::deepMax() */
	decltype(auto) deepMax() const;

	/** @copydoc IndexInterface::reformat() */
	PIndex& reformat(const Vector<SizeT>& f, const Vector<SizeT>& s);

	/** @copydoc IndexInterface::ifor() */
	template <class Xpr, class F>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;

	/** @copydoc IndexInterface::formatIsTrivial() */
	bool formatIsTrivial() const;

	/** @copydoc IndexInterface::xpr() */
	decltype(auto) xpr(const Sptr<PIndex<IndexT>>& _this) const;

	/** Replace instance of index on full range and update current position accordingly.
	    @param i New index instance.
	 */
	PIndex& operator()(const Sptr<IndexT>& i);

	/** Update current index position according to the internal index on the full range. */
	PIndex& operator()();

	/** Get original index.
	    @return Index corresponding to original range and current position. 
	 */
	const Sptr<IndexT>& orig() const;

    private:
	Sptr<RangeType> mRangePtr;
	Sptr<IndexT> mOrig;

	void mkPos();
    };

    /** Make index pack of a PIndex and another index.
	@param a pointer to PIndex.
	@param b pointer to another index.
     */
    template <class I, class I1>
    decltype(auto) operator*(const Sptr<PIndex<I>>& a, const Sptr<I1>& b);

    /** ****
	Specific factory for PRange.
     */
    template <class RangeT>
    class PRangeFactory : public RangeFactoryBase
    {
    public:
	
	/** Construct and setup factory.
	    @param range Full range (explicit type) the constructed range is part of.
	    @param _parts Integer vector indicating the parts w.r.t. input range to be covered by the PRange.
	 */
	PRangeFactory(const Sptr<RangeT>& range, const Vector<SizeT>& _parts);

	/** Construct and setup factory.
	    @param range Full range the constructed range is part of.
	    @param _parts Integer vector indicating the parts of the full range.
	 */
	PRangeFactory(const RangePtr& range, const Vector<SizeT>& _parts);
	
    private:
	PRangeFactory() = default;
	virtual void make() override final;

	Sptr<RangeT> mRange;
	Vector<SizeT> mParts;
    };

    /** ****
	Partial Range.
	Ranges of these kind represent a part of a given range (full range).
	Using a mathematical nomenclature, this would be called a "sub-range".
	(The prefix "sub", as well as the letter "S" are, however, already extensively
	used in other contexts.)
     */
    template <class RangeT>
    class PRange : public RangeInterface<PRange<RangeT>>
    {
    public:
	typedef RangeBase RB;
	typedef PIndex<typename RangeT::IndexType> IndexType;
	typedef typename RangeT::MetaType MetaType;
	
	friend PRangeFactory<RangeT>;

	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	virtual RangePtr extend(const RangePtr& r) const override final;

	/** Get the full range.
	    @return Pointer to the full range.
	 */
	Sptr<RangeT> orig() const;

	/** Get the parts.
	    @return Integer vector indicating the parts contained by the PRange w.r.t. the full range.
	 */
	const Vector<SizeT>& parts() const;

	/** Create a new range of the type of the full range but containing only
	    the parts covered by the PRange.
	    @return The created range.
	 */
	RangePtr derive() const;
	
    private:

	PRange() = delete;
	PRange(const PRange& in) = delete;
	PRange(const Sptr<RangeT>& range, const Vector<SizeT>& _parts);

	virtual Vector<Uuid> key() const override final;
	
	Sptr<RangeT> mRange;
	Vector<SizeT> mParts;
    };

    /** Create a PRange.
	Internally calls PRangeFactory.
	@param range Range to create a PRange on.
	@param parts Integer vector indicating the parts w.r.t. input range to be covered by the PRange.
     */
    template <class RangeT>
    RangePtr prange(const Sptr<RangeT>& range, const Vector<SizeT>& parts);
    
} // namespace CNORXZ

#endif
