// -*- C++ -*-
/**
   
   @file include/ranges/mrange.h
   @brief MRange, GMIndex and MIndex declaration.

   MRange is a multi-range consisting of of a compile-time fixed number of sub-ranges.

   GMIndex and MIndex are multi-index consisting of a compile-time fixed number of sub-indices.
   The difference between the two index types is that MIndex has a statically trivial format,
   while GMIndex can have an arbitrary format (MFormat or GMFormat).
   
   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mrange_h__
#define __cxz_mrange_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"
#include "index_format.h"
#include "index_pack.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    
    /** ****
	Multi-index with fixed number of sub-indices of arbitrary type
	and arbitrary index format.
	@tparam FormatT Index format type.
	@tparam Indices Sub-index types.
     */
    template <class FormatT, class... Indices>
    class GMIndex : public IndexInterface<GMIndex<FormatT,Indices...>,
					  Tuple<typename Indices::MetaType...> >
    {
    public:
	
	typedef IndexInterface<GMIndex<FormatT,Indices...>,
			       Tuple<typename Indices::MetaType...>> IB;
	typedef Tuple<typename Indices::MetaType...> MetaType;
	typedef MRange<typename Indices::RangeType...> RangeType;
	static constexpr SizeT NI = sizeof...(Indices);

	INDEX_RANDOM_ACCESS_ITERATOR_DEFS(MetaType);

	/** Default constructor. */
	constexpr GMIndex() = default;

	/** Move constructor (default). */
	constexpr GMIndex(GMIndex&& i) = default;

	/** Move assignment (default). */
	constexpr GMIndex& operator=(GMIndex&& i) = default;

	/** Copy constructor (no default, copy sub-index instances). */
	constexpr GMIndex(const GMIndex& i);

	/** Copy assignment (no default, copy sub-index instances). */
	constexpr GMIndex& operator=(const GMIndex& i);

	/** Construct from index pack. */
	constexpr GMIndex(const SPack<Indices...>& pack);

	/** Construct from index pack and format. */
	constexpr GMIndex(const FormatT& format, const SPack<Indices...>& pack);

	/** Construct from index pointers. */
	constexpr GMIndex(const Sptr<Indices>&... is);

	/** Construct from index pointers and format. */
	constexpr GMIndex(const FormatT& format, const Sptr<Indices>&... is);

	/** Construct from range. */
	constexpr GMIndex(const RangePtr& range, SizeT lexpos = 0);

	/** Construct from range and format. */
	constexpr GMIndex(const RangePtr& range, const FormatT& format, SizeT lexpos = 0);

	/** @copydoc IndexInterface::operator=(SizeT) */
	GMIndex& operator=(SizeT pos);

	/** @copydoc IndexInterface::operator++() */
	GMIndex& operator++();

	/** @copydoc IndexInterface::operator--() */
	GMIndex& operator--();

	/** @copydoc IndexInterface::operator+() */
	GMIndex operator+(Int n) const;

	/** @copydoc IndexInterface::operator-() */
	GMIndex operator-(Int n) const;

	/** @copydoc IndexInterface::operator-(CIndex) */
	SizeT operator-(const GMIndex& i) const;

	/** @copydoc IndexInterface::operator+=() */
	GMIndex& operator+=(Int n);

	/** @copydoc IndexInterface::operator-=() */
	GMIndex& operator-=(Int n);

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
	GMIndex& at(const MetaType& metaPos);

	/** @copydoc IndexInterface::prange() */
	RangePtr prange(const GMIndex<FormatT,Indices...>& last) const;

	/** @copydoc IndexInterface::deepFormat() */
	auto deepFormat() const;

	/** @copydoc IndexInterface::deepMax() */
	auto deepMax() const;

	/** @copydoc IndexInterface::reformat() */
	GMIndex& reformat(const Vector<SizeT>& f, const Vector<SizeT>& s);

	/** @copydoc IndexInterface::ifor() */
	template <class Xpr, class F>
	constexpr decltype(auto) ifor(const Xpr& xpr, F&& f) const;

	/** @copydoc IndexInterface::formatIsTrivial() */
	bool formatIsTrivial() const;

	/** @copydoc IndexInterface::xpr() */
	decltype(auto) xpr(const Sptr<MIndex<Indices...>>& _this) const;

	// replace sub-index instances; only use if you know what you are doing!
	/** Replace sub-index instances and update index position correspondingly.
	    @param new index instances.
	 */
	GMIndex& operator()(const Sptr<MIndex<Indices...>>& mi);

	/** Update index position according to the sub-indices. */
	GMIndex& operator()();

	/** Get index pack. */
	const SPack<Indices...>& pack() const;

	/** Get index format. */
	const auto& format() const;

	/** Get lexicographic index format. */
	const auto& lexFormat() const;

	/** Assign new index format. */
	GMIndex& setFormat(const FormatT& bs);

    private:
	template <SizeT... Is>
	static constexpr decltype(auto) mkLexFormat(const SPack<Indices...>& ipack, Isq<Is...> is);

	static constexpr decltype(auto) mkLMax(const SPack<Indices...>& ipack);

	static constexpr decltype(auto) mkPMax(const SPack<Indices...>& ipack, const FormatT& format);

	inline void mkPos();
	
	template <SizeT... Is>
	constexpr decltype(auto) mkIPack(Isq<Is...> is) const;

	template <SizeT I>
	inline void up();

	template <SizeT I>
	inline void down();

	template <SizeT I, class Xpr, class F>
	constexpr decltype(auto) mkIFor(const Xpr& xpr, F&& f) const;

	Sptr<RangeType> mRange;
	SPack<Indices...> mIPack;
	typedef RemoveRef<decltype(mkLexFormat(mIPack,Isqr<1,NI>{}))> LexFormatT;
	LexFormatT mLexFormat;
	FormatT mFormat;
	SizeT mLex;
	typedef RemoveRef<decltype(mkLMax(mIPack))> LMaxT;
	LMaxT mLMax;
	typedef RemoveRef<decltype(mkPMax(mIPack,mFormat))> PMaxT;
	PMaxT mPMax;
    };

    /** ****
	Specialization of index_has_const_size for GMIndex.
	@see index_has_const_size
     */
    template <class FormatT, class... Indices>
    struct index_has_const_size<GMIndex<FormatT,Indices...>>
    { static constexpr bool value = (index_has_const_size<Indices>::value and ...); };

    /** ****
	Specialization of index_const_size for GMIndex.
	@see index_const_size
     */
    template <class FormatT, class... Indices>
    struct index_const_size<GMIndex<FormatT,Indices...>>
    { static constexpr SizeT value = (index_const_size<Indices>::value * ...); };

    /** ****
	Specialization of index_dim for GMIndex.
	@see index_dim
     */
    template <class FormatT, class... Indices>
    struct index_dim<GMIndex<FormatT,Indices...>>
    { static constexpr SizeT value = sizeof...(Indices); };

    /** ****
	Specialization of has_sub for GMIndex.
	@see has_sub
     */
    template <class FormatT, class... Indices>
    struct has_sub<GMIndex<FormatT,Indices...>>
    { static constexpr bool value = true; };

    /** ****
	Specialization of has_static_sub for GMIndex.
	@see has_static_sub
     */
    template <class FormatT, class... Indices>
    struct has_static_sub<GMIndex<FormatT,Indices...>>
    { static constexpr bool value = true; };

    /** ****
	Specialization of index_is_multi for GMIndex.
	@see index_is_multi
     */
    template <class FormatT, class... Indices>
    struct index_is_multi<GMIndex<FormatT,Indices...>>
    { static constexpr bool value = true; };

    /** ***
	MIndex can be used as expression if all its sub-indices can be used as expression
	@see index_expression_exists
     */    
    template <class... Indices>
    struct index_expression_exists<MIndex<Indices...>>
    {
	static constexpr bool value = (index_expression_exists<Indices>::value and ...);
    };

    /** Create MIndex from index pointers.
	@param is Input index pointers.
     */
    template <class... Indices>
    constexpr decltype(auto) mindex(const Sptr<Indices>&... is);

    /** Create MIndex from index pack.
	@param pack Pack of input indices.
     */
    template <class... Indices>
    constexpr decltype(auto) mindex(const SPack<Indices...>& pack);

    /** Create pointer to MIndex from index pack.
	@param pack Pack of input indices.
     */
    template <class... Indices>
    constexpr decltype(auto) mindexPtr(const SPack<Indices...>& pack);

    /** Create pointer to GMIndex from index pack and format.
	@param bs Index format.
	@param pack Pack of input indices.
     */
    template <class FormatT, class... Indices>
    constexpr decltype(auto) gmindexPtr(const FormatT& bs, const SPack<Indices...>& pack);

    /** Create pointer to GMIndex from index pack (default format).
	@param pack Pack of input indices.
     */
    template <class... Indices>
    constexpr decltype(auto) gmindexPtr(const SPack<Indices...>& pack);

    /** Specialization for index multiplication with GMIndex on the l.h.s.
	@param a First index of type GMIndex.
	@param b Second index of arbitrary type.
     */
    template <class I1, class FormatT, class... Indices>
    decltype(auto) operator*(const Sptr<GMIndex<FormatT,Indices...>>& a, const Sptr<I1>& b);

    /** ****
	Specific factory for MRange.
	@tparam Ranges Types of the sub-ranges.
     */
    template <class... Ranges>
    class MRangeFactory : public RangeFactoryBase
    {
    public:

	/** Construct and setup factory.
	    @param rs Tuple of sub-ranges.
	 */
	MRangeFactory(const Tuple<Sptr<Ranges>...>& rs);

	/** Construct and setup factory.
	    @param rs Tuple of sub-ranges.
	    @param ref Range the range to be constructed is related to.
	 */
	MRangeFactory(const Tuple<Sptr<Ranges>...>& rs, const RangePtr& ref);
	
    private:
	MRangeFactory() = default;
	virtual void make() override final;

	Tuple<Sptr<Ranges>...> mRs;
	RangePtr mRef;
    };

    /** ****
	Multi Range with compile-time fixed number of sub-ranges
	of compile-time fixed type.
     */
    template <class... Ranges>
    class MRange : public RangeInterface<MRange<Ranges...>>
    {
    public:
	typedef RangeBase RB;
	typedef MIndex<typename Ranges::IndexType...> IndexType;
        typedef Tuple<typename Ranges::IndexType::MetaType...> MetaType;
	static constexpr SizeT NR = sizeof...(Ranges);

	friend MRangeFactory<Ranges...>;

	virtual RangePtr sub(SizeT num) const override final;
	virtual MArray<RangePtr> sub() const override final;
	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	virtual RangePtr extend(const RangePtr& r) const override final;

	/** Get sub-ranges. */
	decltype(auto) space() const;

	/** Get meta data for given lexicographic position.
	    @param pos Lexicographic position.
	 */
	const MetaType get(SizeT pos) const;

	/** Get lexicographic position according to the given meta data value.
	    @param metaPos Meta data value.
	 */
	SizeT getMeta(const MetaType& metaPos) const;

    protected:

	/** Dafault constructor */
	MRange() = default;
	
	MRange(const MRange& in) = delete;
	MRange& operator=(const MRange& in) = delete;

	/** Construct from sub-ranges
	    @param rs Tuple of pointers to sub-ranges.
	 */
	MRange(const Tuple<Sptr<Ranges>...>& rs);
	
	Tuple<Sptr<Ranges>...> mRs; /**< Tuple of pointers to sub-ranges. */
	Arr<RangePtr,NR> mA; /**< Array of abstract base pointers to sub-ranges. Redundant to mRs */

	virtual Vector<Uuid> key() const override final;
    private:

	decltype(auto) mkA() const;
    };

    /** Create MRange pointer.
	@param rs Pointer to sub-ranges. 
     */
    template <class... Ranges>
    RangePtr mrange(const Sptr<Ranges>&... rs);

    /** Create MRange pointer.
	@param rs Pointer to sub-ranges. 
     */
    template <class... Ranges>
    Sptr<MRange<Ranges...>> xplMrange(const Sptr<Ranges>&... rs);

    /** ****
	Specialization of RangeCast for MRange.
	@see RangeCast.
     */
    template <class... Ranges>
    struct RangeCast<MRange<Ranges...>>
    {
	static Sptr<MRange<Ranges...>> func(const RangePtr& r);
    };


}

#endif
