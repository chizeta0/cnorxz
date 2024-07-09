// -*- C++ -*-
/**
   
   @file include/ranges/srange.h
   @brief SRange, SRangeFactory and SIndex declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_srange_h__
#define __cxz_srange_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    /** ****
	Specific index for SRange.
	@tparam MetaT Meta data type.
	@tparam S Static size of the range.
     */
    template <typename MetaT, SizeT S>
    class SIndex : public IndexInterface<SIndex<MetaT,S>,MetaT>
    {
    public:
	typedef IndexInterface<SIndex<MetaT,S>,MetaT> IB;
	typedef SRange<MetaT,S> RangeType;
	typedef MetaT MetaType;

	INDEX_RANDOM_ACCESS_ITERATOR_DEFS(MetaType);
	DEFAULT_MEMBERS(SIndex); /**< default constructors and assignments */

	/** Construct index from range and position.
	    @param range Range to iterate over.
	    @param pos lexicographic position.
	 */
	SIndex(const RangePtr& range, SizeT pos = 0);

	/** @copydoc IndexInterface::operator=(SizeT) */
	SIndex& operator=(SizeT lexpos);

	/** @copydoc IndexInterface::operator++() */
	SIndex& operator++();

	/** @copydoc IndexInterface::operator--() */
	SIndex& operator--();

	/** @copydoc IndexInterface::operator+() */
	SIndex operator+(Int n) const;

	/** @copydoc IndexInterface::operator-() */
	SIndex operator-(Int n) const;

	/** @copydoc IndexInterface::operator-(SIndex) */
	SizeT operator-(const SIndex& i) const;

	/** @copydoc IndexInterface::operator+=() */
	SIndex& operator+=(Int n);

	/** @copydoc IndexInterface::operator-=() */
	SIndex& operator-=(Int n);

	/** @copydoc IndexInterface::lex() */
	SizeT lex() const;

	/** @copydoc IndexInterface::pmax() */
	SPos<S> pmax() const;

	/** @copydoc IndexInterface::lmax() */
	SPos<S> lmax() const;

	/** @copydoc IndexInterface::id() */
	IndexId<0> id() const;
	
	/** @copydoc IndexInterface::operator*() */
	const MetaT& operator*() const;
	
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
	const MetaT& meta() const;

	/** @copydoc IndexInterface::at() */
	SIndex& at(const MetaT& metaPos);

	/** @copydoc IndexInterface::prange() */
	RangePtr prange(const SIndex<MetaType,S>& last) const;

	/** @copydoc IndexInterface::deepFormat() */
	SizeT deepFormat() const;

	/** @copydoc IndexInterface::deepMax() */
	SizeT deepMax() const;

	/** @copydoc IndexInterface::reformat() */
	SIndex& reformat(const Vector<SizeT>& f, const Vector<SizeT>& s);

	/** @copydoc IndexInterface::ifor() */
	decltype(auto) xpr(const Sptr<SIndex<MetaType,S>>& _this) const;

	/** @copydoc IndexInterface::ifor() */
	template <class Xpr, class F>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;

	/** @copydoc IndexInterface::formatIsTrivial() */
	bool formatIsTrivial() const;

    private:
	Sptr<RangeType> mRangePtr;
	const MetaT* mMetaPtr;
    };

    /** Make index pack of a SIndex and another index.
	@param a pointer to SIndex.
	@param b pointer to another index.
     */
    template <typename MetaT, SizeT S, class I1>
    decltype(auto) operator*(const Sptr<SIndex<MetaT,S>>& a, const Sptr<I1>& b);

    /** ****
	Specific factory for SRange.
	@tparam MetaT Meta data type.
	@tparam S Static size of the range.
     */
    template <typename MetaT, SizeT S>
    class SRangeFactory : public RangeFactoryBase
    {
    public:

	/** Construct and setup factory.
	    @param space Meta data array defining the range. 
	 */
	SRangeFactory(const Arr<MetaT,S>& space);

	/** Construct and setup factory.
	    @param space Meta data array defining the range (move). 
	 */
        SRangeFactory(Arr<MetaT,S>&& space);

	/** Construct and setup factory.
	    @param space Meta data array defining the range.
	    @param ref Range the range to be constructed is related to.
	 */
	SRangeFactory(const Arr<MetaT,S>& space, const RangePtr& ref);

	/** Construct and setup factory.
	    @param space Meta data array defining the range (move).
	    @param ref Range the range to be constructed is related to.
	 */
        SRangeFactory(Arr<MetaT,S>&& space, const RangePtr& ref);
	
    private:
	SRangeFactory() = default;
	virtual void make() override final;

	Arr<MetaT,S> mSpace;
	RangePtr mRef;
    };

    /** ****
	Static size range.
	The same as URange, but the range size is compile-time fixed.
	@tparam MetaT Meta data type.
	@tparam S Static range size.
     */
    template <typename MetaT, SizeT S>
    class SRange : public RangeInterface<SRange<MetaT,S>>
    {
    public:
	typedef RangeBase RB;
	typedef SIndex<MetaT,S> IndexType;
	typedef MetaT MetaType;

	friend SRangeFactory<MetaT,S>;

	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	virtual RangePtr extend(const RangePtr& r) const override final;

	/** return meta data at given position.
	    @param pos position, size type
	 */
	const MetaT& get(SizeT pos) const;

	/** Get underlying meta data array.
	    @return Pointer to first position of meta data array. 
	 */
	const MetaT* get() const;

	/** return position for given meta data.
	    @param metaPos meta data, size type
	 */
	SizeT getMeta(const MetaT& metaPos) const;

    private:

	SRange() = default;
	SRange(const SRange& in) = delete;
	SRange(const Arr<MetaT,S>& space);
	SRange(Arr<MetaT,S>&& space);

	Arr<MetaT,S> mSpace;

	virtual Vector<Uuid> key() const override final;

	SERIALIZATION_FUNCTIONS_NOPUB;
    };

    /** ***
	Specialize RangeCast for casts to SRange
	@see RangeCast
	@tparam MetaT Meta data type.
	@tparam S Static range size.
     */    
    template <typename MetaT, SizeT S>
    struct RangeCast<SRange<MetaT,S>>
    {
	/** cast the range */
	static Sptr<SRange<MetaT,S>> func(const RangePtr& r);
    };
    
    /** ***
	SIndex can be used as expression
	@see index_expression_exists
     */    
    template <typename MetaT, SizeT S>
    struct index_expression_exists<SIndex<MetaT,S>>
    {
	static constexpr bool value = true;
    };
}

#endif
