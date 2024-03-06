// -*- C++ -*-
/**
   
   @file opt/mpi/include/rrange.h
   @brief RRange and RIndex declaration

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_rrange_h__
#define __cxz_mpi_rrange_h__

#include "cnorxz.h"

namespace CNORXZ
{
    /** ****
	Specific index for RRanges.
     */
    class RIndex : public IndexInterface<RIndex<Vector<SizeT>>>
    {
    public:
	typedef IndexInterface<RIndex<Vector<SizeT>>> IB;
	typedef RRange RangeType;
	typedef Vector<SizeT> MetaType;

	INDEX_RANDOM_ACCESS_ITERATOR_DEFS(MetaType);

	/** Default constructor. */
	RIndex() = default;

	/** Move constructor. */
	RIndex(RIndex&& i) = default;

	/** Move assignment. */
	RIndex& operator=(RIndex&& i) = default;

	/** Copy constructor.
	    No default copy: Have to copy sub-index instances
	 */
	RIndex(const RIndex& i);

	/** Copy assigment.
	    No default copy: Have to copy sub-index instances
	 */
	RIndex& operator=(const RIndex& i);

	/** Construct from a range and an initial lexicographic position
	    @param range Range to iterate over.
	    @param lexpos Initial lexicographic position.
	 */
	RIndex(const RangePtr& range, SizeT lexpos = 0);

	/** @copydoc IndexInterface::operator=(SizeT) */
	RIndex& operator=(SizeT lexpos);

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
	RIndex& at(const Vector<DType>& meta);

	/** @copydoc IndexInterface::prange() */
	RangePtr prange(const RIndex& last) const;

	/** @copydoc IndexInterface::deepFormat() */
	Vector<SizeT> deepFormat() const;

	/** @copydoc IndexInterface::deepMax() */
	Vector<SizeT> deepMax() const;

	/** @copydoc IndexInterface::reformat() */
	RIndex& reformat(const Vector<SizeT>& f, const Vector<SizeT>& s);

	/** @copydoc IndexInterface::ifor() */
	DXpr<None> ifor(const DXpr<None>& xpr, NoF&& f) const;

	/** @copydoc IndexInterface::formatIsTrivial() */
	bool formatIsTrivial() const;

	/** Replace sub-index instances.
	    All linearized positions are updated accordingly.
	    @param i Pointer to RIndex which provides the new sub-index instance 
	 */
	RIndex& operator()(const Sptr<RIndex>& i);

	/** Update all linearized positions. */
	RIndex& operator()();

	/** Get all sub-indices
	    @return Pack of sub-indices
	 */
	const CPack& pack() const;

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
	RIndex& setFormat(const YFormat& bs);

	/** Set position of given sub index and update total index position.
	    @param ind Sub-index number [0,dim()-1].
	    @param lex Lexicographic position to be assigned to the index.
	 */
	RIndex& setSub(SizeT ind, SizeT lex);
	
    private:
	Sptr<RRange> mRange;
	Vector<Sptr<CIndex>> mIs; // -> CPack!!!
	YFormat mLexFormat; // = mFormat (rank geometry is fixed and unique)
	UPos mPMax = 0; // = mLMax (same reason)
    }

    /** ****
	Specialization: RIndex is a multi-index.
	@see index_is_multi
     */
    template <>
    struct index_is_multi<RIndex>
    { static constexpr bool value = true; };

    /** ****
	Specialization: RIndex has sub-indices.
	@see has_sub
     */
    template <>
    struct has_sub<RIndex>
    { static constexpr bool value = true; };

    /** ****
	Specific factory for RRange.
     */
    class RRangeFactory : public RangeFactoryBase
    {
    public:
	/** Construct and setup factory.
	    @param geom SizeT vector specifying the rank geometry
	 */
	RRangeFactory(const Vector<SizeT>& geom);

    private:
	RRangeFactory() = default;
	virtual void make() override final;

	MArray<RangePtr> mRA;
    };

    /** ****
	Multi-dimensional range specifying the rank coordinate system
     */
    class RRange : public RangeInterface<RRange>
    {
    public:
	typedef RangeBase RB;
	typedef RIndex IndexType;

	friend RRangeFactory;
	
	virtual RangePtr sub(SizeT i) const override final;
	virtual MArray<RangePtr> sub() const override final;
	virtual SizeT size() const override final;
	virtual SizeT dim() const override final;
	virtual String stringMeta(SizeT pos) const override final;
	virtual const TypeInfo& type() const override final;
	virtual const TypeInfo& metaType() const override final;
	virtual RangePtr extend(const RangePtr& r) const override final;

	int myrank() const;
	
    private:

	RRange() = default;
	RRange(const RRange& a) = delete;
	RRange(const MArray<RangePtr>& rvec);

	MArray<RangePtr> mRA;
	int mMyRank = 0;

	virtual Vector<Uuid> key() const override final;
    };
}

#endif
