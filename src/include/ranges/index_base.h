// -*- C++ -*-
/**
   
   @file include/ranges/index_base.h
   @brief Index base declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_index_base_h__
#define __cxz_index_base_h__

#include "base/base.h"
#include "range_base.h"
#include "xpr/index_id.h"
#include "xpr/xpr_base.h"
#include "xpr/func.h"

namespace CNORXZ
{
    /** ******
	Basic index interface class template

	Static polymorphism: This class defines an interface template;
	all indices are supposed to be derived from the corresponding
	template instance

	Indices act as iterators over parameter spaces (ranges) and/or
	the containers (arrays) defined on them

	Moreover, indices are used to define operations on the
	corresponding parameters space

	In principle, one distinguishes between lexicographic position
	(= index position) and the memory position of the corresponding
	meta data w.r.t. the first element
	
	@tparam I Index type, recurring template argument
	@tparam MetaType data type of the meta data

	@see RangeBase
	@see RangeInterface
     */
    template <class I, typename MetaType>
    class IndexInterface
    {
    public:
	/** default destructor */
	~IndexInterface() = default; 

	/** explicit static cast */
	I& THIS() { return static_cast<I&>(*this); }

	/** explicit static cast (const) */
	const I& THIS() const { return static_cast<const I&>(*this); }

	/** increment lexicographic index position */
	I& operator++() { return ++THIS(); }

	/** decrement lexicographic index position */
	I& operator--() { return --THIS();}

	/** return index with lexicographic index position incremented n times
	    @param n
	 */
	I operator+(Int n) const { return THIS() + n; }

	/** return index with lexicographic index position decremented n times
	    @param n
	 */
	I operator-(Int n) const { return THIS() - n; }

	/** increment lexicographic index position n times
	    @param n
	 */
	I& operator+=(Int n) { return THIS() += n; }

	/** decrement lexicographic index position n times
	    @param n
	 */
	I& operator-=(Int n) { return THIS() -= n; }

	/** difference of lexicographic index postions
	    @param i Index to compare with
	*/
	Int operator-(const IndexInterface& i) const { return lex() - i.lex(); }

	/** memory position */
	SizeT pos() const;

	/** lexicographic position */
	SizeT lex() const { return THIS().lex(); }

	/** maximal memory position */
	SizeT pmax() const { return static_cast<SizeT>(THIS().pmax()); }

	/** maximal lexicographic position */
	SizeT lmax() const { return static_cast<SizeT>(THIS().lmax()); }

	PtrId ptrId() const;

	/** index ID */
	decltype(auto) id() const { return THIS().id(); }

	/** check if indices are equal
	    @param i Index to compare with
	 */
	bool operator==(const IndexInterface& i) const;

	/** check if indices are not equal
	    @param i Index to compare with
	 */
	bool operator!=(const IndexInterface& i) const;

	/** check if index position is less than that of i
	    @param i Index to compare with
	 */
	bool operator<(const IndexInterface& i) const;

	/** check if index position is greater than that of i
	    @param i Index to compare with
	 */
	bool operator>(const IndexInterface& i) const;

	/** check if index position is less or equal than that of i
	    @param i Index to compare with
	 */
	bool operator<=(const IndexInterface& i) const;

	/** check if index position is greater or equal than that of i
	    @param i Index to compare with
	 */
	bool operator>=(const IndexInterface& i) const;

	/** return meta data at current index position */
	decltype(auto) operator*() const { return THIS().operator*(); }

	/** index dimension */
	SizeT dim() const { return THIS().dim(); }

	/** pointer to the range the index is defined on */
	decltype(auto) range() const { return THIS().range(); }

	/** return the memory step size this index has w.r.t. to index of given ID

	    @tparam J static index ID, size type
	    @param id input index ID
	 */
	template <SizeT J>
	decltype(auto) stepSize(const IndexId<J>& id) const { return THIS().stepSize(id); }

	/** return string formatted meta data at current index position*/
	String stringMeta() const { return THIS().stringMeta(); }

	/** return meta data at current index position */
	decltype(auto) meta() const { return THIS().meta(); }

	/** set index position so that it's meta data matches input

	    @param meta meta data to be matched
	 */
	I& at(const MetaType& meta) { return THIS().at(meta); }

	/** create partial range starting at this index' position and ending
	    at the position of input index. The end position is included!
	    @param end end index
	 */
	RangePtr prange(const IndexInterface& end) const { return THIS().prange(end.THIS()); }

	/** recursive index format */
	decltype(auto) deepFormat() const { return THIS().deepFormat(); }

	/** max of all single-indices (recursive) */
	decltype(auto) deepMax() const { return THIS().deepMax(); }
	
	/** reformat index, create new index instance
	    @param f new format
	    @param s new sub-index sizes
	 */
	I& reformat(const Vector<SizeT>& f, const Vector<SizeT>& s) { return THIS().reformat(f,s); }
	
	/** create a for-loop expression

	    @tparam Xpr loop internal expression
	    @tparam F accumulating function
	    @param xpr loop internal expression
	    @param f accumulating function
	 */
	template <class Xpr, class F = NoF>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const
	{ return THIS().ifor(xpr,std::forward<F>(f)); }

	/** check is format is trivial */
	bool formatIsTrivial() const { return THIS().formatIsTrivial(); }
	
    protected:
	SizeT mPos = 0; /**< the memory position */
	
    private:
	friend I; // not protected; I should really be something that is derived from IndexInterface
	
	// NO DEFAULT CONSTRUCTORS/ASSIGNMENTS!
	IndexInterface();
	IndexInterface(const IndexInterface& in);
	IndexInterface& operator=(const IndexInterface& in);
	IndexInterface(IndexInterface&& in);
	IndexInterface& operator=(IndexInterface&& in);
	IndexInterface(SizeT pos);

	PtrId mPtrId = 0;
    };

    
    template <class I, typename MetaType>
    IndexPtr<I,MetaType>& operator++(const IndexPtr<I,MetaType>& i);

    template <class I, typename MetaType>
    IndexPtr<I,MetaType>& operator--(const IndexPtr<I,MetaType>& i);
    
    // to define relative indices:
    template <class I, typename MetaType>
    IndexPtr<I,MetaType> operator+(const IndexPtr<I,MetaType>& i, Int n);

    template <class I, typename MetaType>
    IndexPtr<I,MetaType> operator-(const IndexPtr<I,MetaType>& i, Int n);

    template <class I>
    Sptr<I> moveToPtr(I&& i);

    template <class I>
    decltype(auto) xpr(const Sptr<I>& i);

}

#endif
