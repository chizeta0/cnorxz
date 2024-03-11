// -*- C++ -*-
/**
   
   @file opt/mpi/include/rrange.h
   @brief RRange and RIndex declaration.
   
   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_rrange_h__
#define __cxz_mpi_rrange_h__

#include "cnorxz.h"

namespace CNORXZ
{
    namespace mpi
    {

	/** ****
	    Specific index for RRange.
	    Every call on an instance of this class has to be done on each rank.
	    @tparam IndexI Index type used to indicate local position.
	    @tparam IndexK Index type used to indicate the rank.
	*/
	template <class IndexI, class IndexK>
	class RIndex : public IndexInterface<RIndex<IndexI,IndexK>,typename Index::MetaType>
	{
	public:
	    typedef IndexInterface<RIndex<IndexI,IndexK>,typename Index::MetaType> IB;
	    typedef typename Index::MetaType MetaType;
	    typedef RRange<typename Index::RangeType> RangeType;

	    INDEX_RANDOM_ACCESS_ITERATOR_DEFS(MetaType);

	    // constructors!!!
	    /** Default constructor. */
	    RIndex() = default;

	    /** Move constructor (default). */
	    RIndex(RIndex&& in) = default;

	    /** Move assignment (default). */
	    RIndex& operator=(RIndex&& in) = default;

	    /** Copy constructor (no default, copy local index instance). */
	    RIndex(const RIndex& in);

	    /** Copy assignment (no default, copy local index instance). */
	    RIndex& operator=(const RIndex& in);

	    /** Construct from global range and format.
		@param global Pointer to global range; this can be a RRange or the bare range (will be converted to a RRange).
		@param lexpos Start position.
	     */
	    RIndex(const RangePtr& global, SizeT lexpos = 0);

	    /** Construct from local index. */
	    RIndex(const Sptr<IndexI,IndexK>& local);
	    
	    /** @copydoc IndexInterface::operator=(SizeT) */
	    RIndex& operator=(SizeT pos);

	    /** @copydoc IndexInterface::operator++() */
	    RIndex& operator++();

	    /** @copydoc IndexInterface::operator--() */
	    RIndex& operator--();

	    /** @copydoc IndexInterface::operator+() */
	    RIndex operator+(Int n) const;

	    /** @copydoc IndexInterface::operator-() */
	    RIndex operator-(Int n) const;

	    /** @copydoc IndexInterface::operator-(CIndex) */
	    SizeT operator-(const RIndex& i) const;

	    /** @copydoc IndexInterface::operator+=() */
	    RIndex& operator+=(Int n);

	    /** @copydoc IndexInterface::operator-=() */
	    RIndex& operator-=(Int n);

	    /** @copydoc IndexInterface::lex() */
	    SizeT lex() const;

	    /** @copydoc IndexInterface::pmax() */
	    constexpr decltype(auto) pmax() const;

	    /** @copydoc IndexInterface::lmax() */
	    constexpr decltype(auto) lmax() const;

	    /** @copydoc IndexInterface::id() */
	    IndexId<0> id() const;

	    /** @copydoc IndexInterface::operator*() */
	    MetaType operator*() const;
	
	    /** @copydoc IndexInterface::dim() */
	    constexpr SizeT dim() const;

	    /** @copydoc IndexInterface::range() */
	    Sptr<RangeType> range() const;

	    /** @copydoc IndexInterface::stepSize() */
	    template <SizeT I>
	    decltype(auto) stepSize(const IndexId<I>& id) const;

	    /** @copydoc IndexInterface::stringMeta() */
	    String stringMeta() const;

	    /** @copydoc IndexInterface::meta() */
	    MetaType meta() const;

	    /** @copydoc IndexInterface::at() */
	    RIndex& at(const MetaType& metaPos);

	    /** @copydoc IndexInterface::prange() */
	    RangePtr prange(const RIndex<IndexI,IndexK>& last) const;

	    /** @copydoc IndexInterface::deepFormat() */
	    auto deepFormat() const;

	    /** @copydoc IndexInterface::deepMax() */
	    auto deepMax() const;

	    /** @copydoc IndexInterface::reformat() */
	    RIndex& reformat(const Vector<SizeT>& f, const Vector<SizeT>& s);

	    /** @copydoc IndexInterface::ifor() */
	    template <class Xpr, class F>
	    constexpr decltype(auto) ifor(const Xpr& xpr, F&& f) const;

	    /** @copydoc IndexInterface::formatIsTrivial() */
	    bool formatIsTrivial() const;

	    /** @copydoc IndexInterface::xpr() */
	    decltype(auto) xpr(const Sptr<RIndex<IndexI,IndexK>>& _this) const;

	    /** Get the current rank. */
	    int rank() const;

	    /** Get the local index on THIS rank. */
	    Sptr<IndexI,IndexK> local() const;
	    //!!!
	
	private:
	    Sptr<RangeType> mRange; /**< RRange. */
	    Sptr<IndexI> mJ; /**< Index on the local range of the THIS rank. */
	    Sptr<IndexK> mK; /**< Multi-index indicating the current rank. */
	    //!!!
	};

	// Factory!!!

	/** ****
	    Range-Wrapper for ranges that are distributed on MPI ranks.
	    @tparam Range Local range type.
	*/
	template <class Range>
	class RRange : public RangeInterface<RRange<Range>>
	{
	public:
	    typedef RangeBase RB;
	    typedef RIndex<typename Range::IndexType> IndexType;
	    typedef typename Range::MetaType MetaType;

	    friend RRangeFactory<Range>;

	    virtual RangePtr sub(SizeT num) const override final;
	    virtual MArray<RangePtr> sub() const override final;
	    virtual SizeT size() const override final;
	    virtual SizeT dim() const override final;
	    virtual String stringMeta(SizeT pos) const override final;
	    virtual const TypeInfo& type() const override final;
	    virtual const TypeInfo& metaType() const override final;
	    virtual RangePtr extend(const RangePtr& r) const override final;

	    /** Get local range. */
	    Sptr<Range> local() const;

	    /** Get range of the rank geometry. */
	    Sptr<YRange> geom() const;
	    
	    /** Get meta data for given lexicographic position.
		@param pos Lexicographic position.
	    */
	    const MetaType get(SizeT pos) const;

	    /** Get lexicographic position according to the given meta data value.
		@param metaPos Meta data value.
	    */
	    SizeT getMeta(const MetaType& metaPos) const;

	    /** Get rank from lexicographic meta data position.
		@param pos Lexicographic meta data position.
	    */
	    int getRank(SizeT pos) const;

	protected:

	    /** Dafault constructor */
	    RRange() = default;

	    RRange(const RRange& in) = delete;
	    RRange& operator=(const RRange& in) = delete;

	    /** Construct from local range and geometry.
		@param loc Local range.
		@param geom Rank geometry range.
	    */
	    RRange(const Sptr<Range>& loc, const Sptr<YRange>& geom);
	
	    Sptr<Range> mLocal; /**< Local range of THIS rank. */
	    Sptr<YRange> mGeom; /**< Rank geometry range. */
	
	};

	/** Create RRange from global range and given rank geometry.
	    @param global Global range.
	    @param geom Rank geometry.
	*/
	template <class GRange>
	RangePtr rrange(const Sptr<GRange>& global, const Sptr<YRange>& geom);
	
    } // namespace mpi
} // namespace CNORXZ

#endif
