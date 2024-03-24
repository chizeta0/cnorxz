// -*- C++ -*-
/**
   
   @file opt/mpi/include/rrange.h
   @brief RRange and RIndex declaration.
   
   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_rrange_h__
#define __cxz_mpi_rrange_h__

#include "mpi_base.h"

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
	class RIndex : public IndexInterface<RIndex<IndexI,IndexK>,typename IndexI::MetaType>
	{
	public:
	    typedef IndexInterface<RIndex<IndexI,IndexK>,typename IndexI::MetaType> IB;
	    typedef typename IndexI::MetaType MetaType;
	    typedef RRange<typename IndexI::RangeType,typename IndexK::RangeType> RangeType;

	    INDEX_RANDOM_ACCESS_ITERATOR_DEFS(MetaType);

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
		@param global Pointer to global range (RRange).
		@param lexpos Start position.
	     */
	    RIndex(const RangePtr& global, SizeT lexpos = 0);

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

	    /** Update index position according to the sub-indices. */
	    RIndex& operator()();
	    
	    /** Get the current rank. */
	    SizeT rank() const;

	    /** Get the local index on THIS rank. */
	    Sptr<IndexI> local() const;
	
	private:
	    SizeT mLex = 0;
	    Sptr<RangeType> mRange; /**< RRange. */
	    Sptr<IndexI> mI; /**< Index on the local range of the THIS rank. */
	    Sptr<IndexK> mK; /**< Multi-index indicating the current rank. */
	};

	// Traits!!!
	template <class I>
	struct is_rank_index
	{
	    static constexpr bool value = false;
	};

	template <class IndexI, class IndexK>
	struct is_rank_index<RIndex<IndexI,IndexK>>
	{
	    static constexpr bool value = true;
	};
	
	/** ****
	    Specific factory for RRange.
	    @tparam RangeI Local range type.
	    @tparam RangeK Geometry range type.
	 */
	template <class RangeI, class RangeK>
	class RRangeFactory : public RangeFactoryBase
	{
	public:
	    /** Construct and setup factory.
		@param ri Local range.
		@param rk Geometry range.
	     */
	    RRangeFactory(const Sptr<RangeI>& ri, const Sptr<RangeK>& rk);
	    
	private:
	    RRangeFactory() = default;
	    virtual void make() override final;

	    Sptr<RangeI> mRI;
	    Sptr<RangeK> mRK;
	};	

	/** ****
	    Range-Wrapper for ranges that are distributed on MPI ranks.
	    @tparam RangeI Local range type.
	    @tparam RangeK Geometry range type.
	*/
	template <class RangeI, class RangeK>
	class RRange : public RangeInterface<RRange<RangeI,RangeK>>
	{
	public:
	    typedef RangeBase RB;
	    typedef RIndex<typename RangeI::IndexType,typename RangeK::IndexType> IndexType;
	    typedef typename RangeI::MetaType MetaType;

	    friend RRangeFactory<RangeI,RangeK>;

	    virtual RangePtr sub(SizeT num) const override final;
	    virtual MArray<RangePtr> sub() const override final;
	    virtual SizeT size() const override final;
	    virtual SizeT dim() const override final;
	    virtual String stringMeta(SizeT pos) const override final;
	    virtual const TypeInfo& type() const override final;
	    virtual const TypeInfo& metaType() const override final;
	    virtual RangePtr extend(const RangePtr& r) const override final;

	    /** Get local range. */
	    Sptr<RangeI> local() const;

	    /** Get range of the rank geometry. */
	    Sptr<RangeK> geom() const;
	    
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
	    SizeT getRank(SizeT pos) const;

	protected:

	    /** Dafault constructor */
	    RRange() = default;

	    RRange(const RRange& in) = delete;
	    RRange& operator=(const RRange& in) = delete;

	    /** Construct from local range and geometry.
		@param loc Local range.
		@param geom Rank geometry range.
	    */
	    RRange(const Sptr<RangeI>& loc, const Sptr<RangeK>& geom);
	
	    Sptr<RangeI> mLocal; /**< Local range of THIS rank. */
	    Sptr<RangeK> mGeom; /**< Rank geometry range. */
	
	    virtual Vector<Uuid> key() const override final;
	};

	/** Create RRange from global range and given rank geometry.
	    @param global Global range.
	    @param geom Rank geometry.
	*/
	RangePtr rrange(const RangePtr& global, const RangePtr& geom);

    } // namespace mpi

    template <class RangeI, class RangeK>
    struct RangeCast<mpi::RRange<RangeI,RangeK>>
    {
	static Sptr<mpi::RRange<RangeI,RangeK>> func(const RangePtr& r);
    };
	
} // namespace CNORXZ

#endif
