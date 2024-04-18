// -*- C++ -*-
/**
   
   @file include/array/array_base.h
   @brief Array base class declarations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_array_base_h__
#define __cxz_array_base_h__

#include <cstdlib>
#include <vector>
#include <memory>
#include <algorithm>

#include "base/base.h"
#include "aindex.h"
#include "operation/op_types.h"

namespace CNORXZ
{
    /** ****
	Abstract container base class
	only read access to the data
	
	@tparam T data type
     */
    template <typename T>
    class CArrayBase
    {
    public:
	typedef AIndex<T> const_iterator; /** const iterator type */

	DEFAULT_MEMBERS(CArrayBase); /**< default constructors and assignments */

	/** construct container on a range
	    @param range 
	 */
	CArrayBase(const RangePtr& range);

	/** default destructor */
	virtual ~CArrayBase() = default;

	/** Return unique pointer copy of itself. */
	virtual Uptr<CArrayBase> copy() const = 0;
	
	/** const data element access.
	    If the array's format is trivial or the index is already non-trivially formatted,
	    this is equivalent to data()[i.pos()]. Otherwise, the array's format is applied.
	    @tparam I index type
	    @tparam M meta data type
	    @param i index
	 */
	template <typename I, typename M>
	const T& operator[](const IndexInterface<I,M>& i) const;

	/** const data element access
	    performs compatibility checks 
	    @tparam I index type
	    @tparam M meta data type
	    @param i index
	 */
	template <typename I, typename M>
	const T& at(const IndexInterface<I,M>& i) const;

	/** const data element access
	    @tparam I index type
	    @tparam M meta data type
	    @param pack static index pack
	 */
	template <class... Indices>
	const T& operator[](const SPack<Indices...>& pack) const;

	/** const data element access
	    performs compatibility checks 
	    @tparam I index type
	    @tparam M meta data type
	    @param i static index pack
	 */
	template <class... Indices>
	const T& at(const SPack<Indices...>& pack) const;

	/** const data element access
	    @param pack index pack
	 */
	const T& operator[](const DPack& pack) const;

	/** const data element access
	    performs compatibility checks 
	    @param i index pack
	 */
	const T& at(const DPack& pack) const;

	/** create hypercubic slice from this container
	    @tparam I type of index used to indicate slice edges
	    @tparam M index meta type
	    @param begin begin edge
	    @param end end edge
	 */
	template <typename I, typename M>
	Sptr<CArrayBase<T>> sl(const IndexInterface<I,M>& begin,
			       const IndexInterface<I,M>& end) const;

	/** create operation on this container.
	    Caution: might modify the index format.
	    @tparam Index type of operation index
	    @param i operation index
	 */
	template <class Index>
	COpRoot<T,Index> operator()(const Sptr<Index>& i) const;

	/** create operation on this container
	    @tparam Indices types of operation indices
	    @param pack pack of operation index
	 */
	template <class... Indices>
	inline decltype(auto) operator()(const SPack<Indices...>& pack) const;

	/** create operation on this container
	    @param pack pack of operation index
	 */
	inline decltype(auto) operator()(const DPack& pack) const;

	/** get pointer to container data */
	virtual const T* data() const = 0;

	/** get number of elements in the container */
	virtual SizeT size() const;

	/** get container range */
	virtual RangePtr range() const;

	/** get index pointing to first position */
	virtual const_iterator begin() const;

	/** get index pointing to position after last position */
	virtual const_iterator end() const;

	/** get index pointing to first position */
	virtual const_iterator cbegin() const = 0;

	/** get index pointing to position after last position */
	virtual const_iterator cend() const = 0;

	/** check if container views the data, i.e. it does not own it */
	virtual bool isView() const = 0;

	/** Perform compatibility checks
	    @tparam Acc index type or index pack type
	    @param acc index or index pack.
	 */
	template <class Acc>
	void checkFormatCompatibility(const Acc& acc) const;

	/** check if format is trivial
	    @return true if container is data owning array, else return
	    result of the corresponding container index
	 */
	virtual bool formatIsTrivial() const = 0;

    protected:
	RangePtr mRange; /**< the container range */

	/** Get valid data index.
	    Create well-formated index from index pack (unformatted)
	    or index using trivial format.
	    @tparam Acc index type or index pack type
	    @param acc index or index pack
	 */
	template <class Acc>
	const_iterator itLex(const Acc& acc) const;

	/** Get valid data index.
	    Create well-formated index from index pack (unformatted)
	    or index using trivial format.
	    Perform compatibility checks.
	    @tparam Acc index type or index pack type
	    @param acc index or index pack
	 */
	template <class Acc>
	const_iterator itLexSave(const Acc& acc) const;

    };

    /** ****
	Abstract container base class
	read and write access to the data

	@tparam T data type
     */
    template <typename T>
    class ArrayBase : public CArrayBase<T>
    {
    public:
	typedef CArrayBase<T> CAB;
	typedef typename CAB::const_iterator const_iterator; /**< constant iterator type */
	typedef BIndex<T> iterator; /**< read/write iterator type */

	using CAB::operator[];
	using CAB::operator();
	using CAB::at;
	using CAB::data;
	using CAB::begin;
	using CAB::end;
	using CAB::cbegin;
	using CAB::cend;
	using CAB::sl;

	DEFAULT_MEMBERS(ArrayBase); /**< default constructors and assignments */

	/** construct a container on a range
	 */
	ArrayBase(const RangePtr& range);

	/** @copydoc CArrayBase::operator[]()
	 */
	template <typename I, typename M>
	T& operator[](const IndexInterface<I,M>& i);

	/** @copydoc CArrayBase::at()
	 */
	template <typename I, typename M>
	T& at(const IndexInterface<I,M>& i);

	/** @copydoc CArrayBase::operator[]()
	 */
	template <class... Indices>
	T& operator[](const SPack<Indices...>& pack);

	/** @copydoc CArrayBase::at()
	 */
	template <class... Indices>
	T& at(const SPack<Indices...>& pack);

	/** @copydoc CArrayBase::operator[]()
	 */
	T& operator[](const DPack& pack);

	/** @copydoc CArrayBase::at()
	 */
	T& at(const DPack& pack);

	/** @copydoc CArrayBase::operator()()
	 */
	template <class Index>
	OpRoot<T,Index> operator()(const Sptr<Index>& i);

	/** @copydoc CArrayBase::operator()()
	 */
	template <class... Indices>
	inline decltype(auto) operator()(const SPack<Indices...>& pack);

	/** @copydoc CArrayBase::operator()()
	 */
	inline decltype(auto) operator()(const DPack& pack);

	/** @copydoc CArrayBase::sl()
	 */
	template <typename I, typename M>
	Sptr<ArrayBase<T>> sl(const IndexInterface<I,M>& begin,
			      const IndexInterface<I,M>& end);

	/** @copydoc CArrayBase::data()
	    read/write access
	 */
	virtual T* data() = 0;
	
	/** @copydoc CArrayBase::begin()
	    read/write access
	 */
	virtual iterator begin();

	/** @copydoc CArrayBase::end()
	    read/write access
	 */
	virtual iterator end();
	
    protected:
	
	/** @copydoc CArrayBase::itLex()
	 */
	template <class Acc>
	iterator itLex(const Acc& acc);

	/** @copydoc CArrayBase::itLexSave()
	 */
	template <class Acc>
	iterator itLexSave(const Acc& acc);
    };

}

#endif
