// -*- C++ -*-
/**
   
   @file opt/mpi/include/raindex.h
   @brief RAIndex declarations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_raindex_h__
#define __cxz_mpi_raindex_h__

#include "cnorxz.h"

namespace CNORXZ
{

    /** ****
	Index for multi-ranked array.
	This is meant to be a global index. For local iterations
	a YIndex is sufficient.
	@tparam T data type.
     */
    template <typename T>
    class RAIndex : public IndexInterface<RAIndex,Tuple<SizeT,Vector<DType>>>
    {
    public:
	typedef IndexInterface<RAIndex,Tuple<SizeT,Vector<DType>>> IB;
	typedef MRange<RRange,YRange> RangeType;
	typedef Tuple<SizeT,Vector<DType>> MetaType;

	INDEX_RANDOM_ACCESS_ITERATOR_DEFS(MetaType);

	/** Default constructor. */
	RAIndex() = default;

	/** Move constructor. */
	RAIndex(RAIndex&& i) = default;

	/** Move assignment. */
	RAIndex& operator=(RAIndex&& i) = default;

	/** Copy constructor.
	    No default copy: Have to copy sub-index instances
	 */
	RAIndex(const RAIndex& i);

	/** Copy assigment.
	    No default copy: Have to copy sub-index instances
	 */
	RAIndex& operator=(const RAIndex& i);

	/** Construct from sub-index pointers.
	    @param is Vector of XIndex pointers.
	    @param geom Rank geometry.
	 */
	RAIndex(const Vector<XIndexPtr>& is, const Vector<SizeT>& geom);

	/** Construct from sub-index pointers, specify index format.
	    @param bs Index format (YFormat).
	    @param is Vector of XIndex pointers.
	    @param geom Rank geometry.
	 */
	RAIndex(const YFormat& bs, const Vector<XIndexPtr>& is, const Vector<SizeT>& geom);

	/** Construct from a range and an initial lexicographic position
	    @param range Range to iterate over.
	    @param geom Rank geometry.
	    @param lexpos Initial lexicographic position.
	 */
	RAIndex(const RangePtr& range, const Vector<SizeT>& geom, SizeT lexpos = 0);

	/** Construct from a range and an initial lexicographic position, specify format.
	    @param range Range to iterate over.
	    @param bs Index format.
	    @param geom Rank geometry.
	    @param lexpos Initial lexicographic position.
	 */
	RAIndex(const RangePtr& range, const YFormat& bs, const Vector<SizeT>& geom, SizeT lexpos = 0);
	
	/** @copydoc IndexInterface::operator=(SizeT) */
	RAIndex& operator=(SizeT lexpos);

	/** @copydoc IndexInterface::operator++() */
	RAIndex& operator++();

	/** @copydoc IndexInterface::operator--() */
	RAIndex& operator--();

	/** @copydoc IndexInterface::operator+() */
	RAIndex operator+(Int n) const;

	/** @copydoc IndexInterface::operator-() */
	RAIndex operator-(Int n) const;

	/** @copydoc IndexInterface::operator-(CIndex) */
	SizeT operator-(const RAIndex& i) const;

	/** @copydoc IndexInterface::operator+=() */
	RAIndex& operator+=(Int n);

	/** @copydoc IndexInterface::operator-=() */
	RAIndex& operator-=(Int n);

	/** @copydoc IndexInterface::lex() */
	SizeT lex() const;

	/** @copydoc IndexInterface::pmax() */
	UPos pmax() const;

	/** @copydoc IndexInterface::lmax() */
	UPos lmax() const;

	/** @copydoc IndexInterface::id() */
	IndexId<0> id() const;
	
	const T& operator*() const;

	const T* operator->() const;

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
	RAIndex& at(const Vector<DType>& meta);

	/** @copydoc IndexInterface::prange() */
	RangePtr prange(const RAIndex& last) const;

	/** @copydoc IndexInterface::deepFormat() */
	Vector<SizeT> deepFormat() const;

	/** @copydoc IndexInterface::deepMax() */
	Vector<SizeT> deepMax() const;

	/** @copydoc IndexInterface::reformat() */
	RAIndex& reformat(const Vector<SizeT>& f, const Vector<SizeT>& s);

	/** @copydoc IndexInterface::ifor() */
	DXpr<None> ifor(const DXpr<None>& xpr, NoF&& f) const;

	/** @copydoc IndexInterface::formatIsTrivial() */
	bool formatIsTrivial() const;

	/** Replace sub-index instances.
	    All linearized positions are updated accordingly.
	    @param i DPack which provides the new sub-index instance 
	 */
	RAIndex& operator()(const DPack& i);

	/** Update all linearized positions. */
	RAIndex& operator()();

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
	RAIndex& setFormat(const YFormat& bs);

	/** Set position of given sub index and update total index position.
	    @param ind Sub-index number [0,dim()-1].
	    @param lex Lexicographic position to be assigned to the index.
	 */
	RAIndex& setSub(SizeT ind, SizeT lex);

    private:
	Sptr<RangeType> mRange;
	MIndex<RIndex,YIndex> mI;
	MFormat<2> mFormat;
	MFormat<2> mLexFormat;
	SizeT mLex = 0;
	UPos mPMax = 0;
	UPos mLMax = 0;
	const T* mLoc = nullptr;
	Vector<T> mBuf; // used if iterating over content on different rank 
    };
    
}

#endif
