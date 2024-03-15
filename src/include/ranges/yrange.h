// -*- C++ -*-
/**
   
   @file include/ranges/yrange.h
   @brief YRange and YIndex declaration

   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_yrange_h__
#define __cxz_yrange_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"
#include "xindex.h"
#include "xpr/xpr.h"
#include "index_format.h"
#include "index_pack.h"
#include "prange.h"

namespace CNORXZ
{
    /** ****
	Specific index for YRanges.

	A YIndex is a multi-index which consists of a set of sub-indices
	and a format. In the case the index is used to access data, this format
	determines the linearized memory position for a given sub-index combination.
     */
    class YIndex : public IndexInterface<YIndex,Vector<DType>>
    {
    public:
	typedef IndexInterface<YIndex,Vector<DType>> IB;
	typedef YRange RangeType;
	typedef Vector<DType> MetaType;
	
	INDEX_RANDOM_ACCESS_ITERATOR_DEFS(MetaType);

	/** Default constructor. */
	YIndex() = default;

	/** Move constructor. */
	YIndex(YIndex&& i) = default;

	/** Move assignment. */
	YIndex& operator=(YIndex&& i) = default;

	/** Copy constructor.
	    No default copy: Have to copy sub-index instances
	 */
	YIndex(const YIndex& i);

	/** Copy assigment.
	    No default copy: Have to copy sub-index instances
	 */
	YIndex& operator=(const YIndex& i);

	/** Construct from sub-index pointers.
	    @param is Vector of XIndex pointers.
	 */
	YIndex(const Vector<XIndexPtr>& is);

	/** Construct from sub-index pointers, specify index format.
	    @param bs Index format (YFormat).
	    @param is Vector of XIndex pointers.
	 */
	YIndex(const YFormat& bs, const Vector<XIndexPtr>& is);

	/** Construct from a range and an initial lexicographic position
	    @param range Range to iterate over.
	    @param lexpos Initial lexicographic position.
	 */
	YIndex(const RangePtr& range, SizeT lexpos = 0);

	/** Construct from a range and an initial lexicographic position, specify format.
	    @param range Range to iterate over.
	    @param bs Index format.
	    @param lexpos Initial lexicographic position.
	 */
	YIndex(const RangePtr& range, const YFormat& bs, SizeT lexpos = 0);

	/** @copydoc IndexInterface::operator=(SizeT) */
	YIndex& operator=(SizeT lexpos);

	/** @copydoc IndexInterface::operator++() */
	YIndex& operator++();

	/** @copydoc IndexInterface::operator--() */
	YIndex& operator--();

	/** @copydoc IndexInterface::operator+() */
	YIndex operator+(Int n) const;

	/** @copydoc IndexInterface::operator-() */
	YIndex operator-(Int n) const;

	/** @copydoc IndexInterface::operator-(CIndex) */
	SizeT operator-(const YIndex& i) const;

	/** @copydoc IndexInterface::operator+=() */
	YIndex& operator+=(Int n);

	/** @copydoc IndexInterface::operator-=() */
	YIndex& operator-=(Int n);

	/** @copydoc IndexInterface::lex() */
	SizeT lex() const;

	/** @copydoc IndexInterface::pmax() */
	UPos pmax() const;

	/** @copydoc IndexInterface::lmax() */
	UPos lmax() const;

	/** @copydoc IndexInterface::id() */
	IndexId<0> id() const;
	
	/** @copydoc IndexInterface::operator*() */
	Vector<DType> operator*() const;

	/** @copydoc IndexInterface::dim() */
	SizeT dim() const;

	/** @copydoc IndexInterface::range() */
	Sptr<YRange> range() const;

	/** @copydoc IndexInterface::stepSize() */
	UPos stepSize(const IndexId<0> id) const;

	/** @copydoc IndexInterface::stringMeta() */
	String stringMeta() const;

	/** @copydoc IndexInterface::meta() */
	Vector<DType> meta() const;

	/** @copydoc IndexInterface::at() */
	YIndex& at(const Vector<DType>& meta);

	/** @copydoc IndexInterface::prange() */
	RangePtr prange(const YIndex& last) const;

	/** @copydoc IndexInterface::deepFormat() */
	Vector<SizeT> deepFormat() const;

	/** @copydoc IndexInterface::deepMax() */
	Vector<SizeT> deepMax() const;

	/** @copydoc IndexInterface::reformat() */
	YIndex& reformat(const Vector<SizeT>& f, const Vector<SizeT>& s);

	/** @copydoc IndexInterface::ifor() */
	DXpr<None> ifor(const DXpr<None>& xpr, NoF&& f) const;

	/** @copydoc IndexInterface::formatIsTrivial() */
	bool formatIsTrivial() const;

	/** Replace sub-index instances.
	    All linearized positions are updated accordingly.
	    @param i Pointer to YIndex which provides the new sub-index instance 
	 */
	YIndex& operator()(const Sptr<YIndex>& i);

	/** Update all linearized positions. */
	YIndex& operator()();

	/** Get all sub-indices
	    @return Pack of sub-indices
	 */
	const DPack& pack() const;

	/** Get index format.
	    @return The format.
	 */
	const YFormat& format() const;

	/** Get lexicographic (trivial) index format.
	    @return The lexicographic format.
	 */
	const YFormat& lexFormat() const;

	/** Set the index format.
	    @param bs The new format.
	 */
	YIndex& setFormat(const YFormat& bs);

	/** Set position of given sub index and update total index position.
	    @param ind Sub-index number [0,dim()-1].
	    @param lex Lexicographic position to be assigned to the index.
	 */
	YIndex& setSub(SizeT ind, SizeT lex);
	
    private:
	inline Vector<SizeT> mkFormat() const;
	inline Vector<SizeT> mkLexFormat() const;
	inline Vector<RangePtr> mkRangeVec(const Vector<XIndexPtr>& is) const;
	inline void mkPos();
	inline DPack mkIndices() const;
	inline void up(SizeT i);
	inline void down(SizeT i);
	inline decltype(auto) mkIFor(SizeT i, const DXpr<None>& xpr, NoF&& f) const;

	inline SizeT mkPMax() const;
	inline SizeT mkLMax() const;
	
	Sptr<YRange> mRange;
	DPack mIs;
	YFormat mFormat; // dim() elements only!
	YFormat mLexFormat; // dim() elements only!
	SizeT mLex = 0;
	UPos mPMax = 0;
	UPos mLMax = 0;
    };

    /** ****
	Specialization: YIndex is a multi-index.
	@see index_is_multi
     */
    template <>
    struct index_is_multi<YIndex>
    { static constexpr bool value = true; };

    /** ****
	Specialization: YIndex has sub-indices.
	@see has_sub
     */
    template <>
    struct has_sub<YIndex>
    { static constexpr bool value = true; };

    /** Create YIndex from an index pack assuming a trivial index format. 
	@param pack Dynamic index pack.
	@return The created YIndex.
     */
    YIndex yindex(const DPack& pack);

    /** Create YIndex from sub-indices assuming a trivial index format. 
	@param is Vector of pointers to the sub-indices used in the YIndex.
	@return The created YIndex.
     */
    YIndex yindex(const Vector<XIndexPtr>& is);

    /** Create YIndex from an index pack assuming a trivial index format. 
	@param pack Dynamic index pack.
	@return A shared pointer to the created YIndex.
     */
    Sptr<YIndex> yindexPtr(const DPack& is);

    /** Create YIndex from sub-indices assuming a trivial index format. 
	@param is Vector of pointers to the sub-indices used in the YIndex.
	@return A shared pointer to the created YIndex.
     */
    Sptr<YIndex> yindexPtr(const Vector<XIndexPtr>& is);

    /** Create YIndex from sub-indices. 
	@param is Vector of pointers to the sub-indices used in the YIndex.
	@param bs Index format.
	@return A shared pointer to the created YIndex.
     */
    Sptr<YIndex> yindexPtr(const Vector<SizeT>& bs, const Vector<XIndexPtr>& is);

    /** ****
	Specific factory for YRange.
     */
    class YRangeFactory : public RangeFactoryBase
    {
    public:

	/** Construct and setup factory.
	    @param rvec Vector of ranges i.e. the sub-ranges the YRange consists of
	 */
	YRangeFactory(const Vector<RangePtr>& rvec);

	/** Construct and setup factory.
	    @param rvec Vector of ranges i.e. the sub-ranges the YRange consists of (move)
	 */
	YRangeFactory(Vector<RangePtr>&& rvec);

	/** Construct and setup factory.
	    @param rvec Vector of ranges i.e. the sub-ranges the YRange consists of
	    @param ref Range the range to be constructed is related to
	 */
	YRangeFactory(const Vector<RangePtr>& rvec, const RangePtr& ref);

	/** Construct and setup factory.
	    @param rvec Vector of ranges i.e. the sub-ranges the YRange consists of (move)
	    @param ref Range the range to be constructed is related to
	 */
	YRangeFactory(Vector<RangePtr>&& rvec, const RangePtr& ref);

    private:
	YRangeFactory() = default;
	virtual void make() override final;

	Vector<RangePtr> mRVec;
	RangePtr mRef;

    };

    /** ****
	Dynamic multi-dimensional range

	Dimension and sub-range types are determined at runtime
	The size of the range is given by the product of the
	sizes of all sub-ranges.
     */
    class YRange : public RangeInterface<YRange>
    {
    public:
	typedef RangeBase RB;
	typedef YIndex IndexType;
	typedef typename IndexType::MetaType MetaType;

	friend YRangeFactory;

	virtual RangePtr sub(SizeT i) const override final;
	virtual MArray<RangePtr> sub() const override final;
	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	virtual RangePtr extend(const RangePtr& r) const override final;

    private:

	YRange() = default;
	YRange(const YRange& a) = delete;
	YRange(const Vector<RangePtr>& rvec);
	YRange(Vector<RangePtr>&& rvec);

	Vector<RangePtr> mRVec;

	virtual Vector<Uuid> key() const override final;
	
	SERIALIZATION_FUNCTIONS_NOPUB;
    };

    /** Create YRange from sub-ranges.
	@param rs Vector of pointers to the sub-ranges used by the YRange.
	@return A shared pointer to the created YRange.
     */
    RangePtr yrange(const Vector<RangePtr>& rs);

    /** Create YRange from sub-ranges.
	@param rs Vector of pointers to the sub-ranges used by the YRange.
	@return A shared pointer to the created YRange.
     */
    Sptr<YRange> xplYrange(const Vector<RangePtr>& rs);

    /** ****
	Specialize RangeCast for casts to YRange.
	@see RangeCast
     */
    template <>
    struct RangeCast<YRange>
    {
	static Sptr<YRange> func(const RangePtr& r);
    };

   
}

#endif
