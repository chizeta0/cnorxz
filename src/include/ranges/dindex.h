// -*- C++ -*-
/**
   
   @file include/ranges/dindex.h
   @brief DIndex declaration

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_dindex_h__
#define __cxz_dindex_h__

#include "base/base.h"
#include "range_base.h"
#include "index_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    /** ****
	index of compile-time-unspecified type
     */
    class DIndex : public IndexInterface<DIndex,DType>
    {
    public:
	typedef IndexInterface<DIndex,DType> IB;
	typedef DType MetaType;
	typedef RangeBase RangeType;

	DEFAULT_C(DIndex); /**< default constructor */

	/** copy constructor
	    @param i index to copy
	 */
	DIndex(const DIndex& i);

	/** move constructor
	    @param i index to move
	 */
	DIndex(DIndex&& i);

	/** copy assignment
	    @param i index to copy 
	 */
	DIndex& operator=(const DIndex& i);

	/** move assignment
	    @param i index to move
	 */
	DIndex& operator=(DIndex&& i);

	/** construct from XIndex instance
	    @param i XIndex to use for construction
	 */
	DIndex(const XIndexPtr& i);

	/** construct from range pointer
	    @param r Range to iterate over
	    @param lexpos initial lexicographic position
	 */
	DIndex(const RangePtr& r, SizeT lexpos = 0);

	/** construct from index of arbitrary type
	    @param i Index to use for construction 
	 */
	template <class Index, typename Meta>
	DIndex(const IndexInterface<Index,Meta>& i);

	/** @copydoc IndexInterface::operator=(SizeT) */
	DIndex& operator=(SizeT lexpos);

	/** @copydoc IndexInterface::operator++() */
	DIndex& operator++();

	/** @copydoc IndexInterface::operator--() */
	DIndex& operator--();

	/** @copydoc IndexInterface::operator+() */
	DIndex operator+(Int n) const;

	/** @copydoc IndexInterface::operator-() */
	DIndex operator-(Int n) const;

	/** @copydoc IndexInterface::operator-(CIndex) */
	SizeT operator-(const DIndex& i) const;

	/** @copydoc IndexInterface::operator+=() */
	DIndex& operator+=(Int n);

	/** @copydoc IndexInterface::operator-=() */
	DIndex& operator-=(Int n);

	/** @copydoc IndexInterface::lex() */
	SizeT lex() const;

	/** @copydoc IndexInterface::pmax() */
	UPos pmax() const;

	/** @copydoc IndexInterface::lmax() */
	UPos lmax() const;

	/** @copydoc IndexInterface::id() */
	IndexId<0> id() const;
	
	/** @copydoc IndexInterface::operator*() */
	DType operator*() const;

	/** @copydoc IndexInterface::dim() */
	SizeT dim() const;

	/** @copydoc IndexInterface::range() */
	RangePtr range() const;

	/** @copydoc IndexInterface::stepSize() */
	UPos stepSize(const IndexId<0>& id) const;

	/** @copydoc IndexInterface::stringMeta() */
	String stringMeta() const;

	/** @copydoc IndexInterface::meta() */
	DType meta() const;

	/** @copydoc IndexInterface::at() */
	DIndex& at(const DType& meta);

	/** @copydoc IndexInterface::prange() */
	RangePtr prange(const DIndex& end) const;

	/** @copydoc IndexInterface::deepFormat() */
	Vector<SizeT> deepFormat() const;

	/** @copydoc IndexInterface::deepMax() */
	Vector<SizeT> deepMax() const;

	/** @copydoc IndexInterface::reformat() */
	DIndex& reformat(const Vector<SizeT>& f, const Vector<SizeT>& s);
	
	/** @copydoc IndexInterface::ifor() */
	template <class Xpr, class F>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;
	
	/** @copydoc IndexInterface::formatIsTrivial() */
	bool formatIsTrivial() const;

	/** get internal XIndex instance
	    @return shared pointer to XIndex instance
	 */
	const XIndexPtr& xptr() const;

    private:
	XIndexPtr mI;
    };

    /** Make index pack of a DIndex and another index.
	@param a pointer to DIndex.
	@param b pointer to another index.
     */
    template <class I>
    decltype(auto) operator*(const Sptr<DIndex>& a, const Sptr<I>& b);

    /** Trait-specialization:
	DIndex can have sub-indices
     */
    template <>
    struct has_sub<DIndex>
    { static constexpr bool value = true; };

    /** dynamically cast DIndex to given type.
	If type does not match the index type of the underlying index
	and exception is thrown

	@tparam I index type to be returned
	@param i DIndex to by casted
	@return copy of underlying index instance
     */
    template <class I>
    I indexAs(const DIndex& i);
}

#endif
