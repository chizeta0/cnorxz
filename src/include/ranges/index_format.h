// -*- C++ -*-
/**
   
   @file include/ranges/index_format.h
   @brief ...


   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_index_format_h__
#define __cxz_index_format_h__

#include "base/base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    /** ****
	Type trait: Check if format has static size.
     */
    template <class FormatT> struct is_static_format { CXZ_CVAL_FALSE; };

    /** ****
	Multi index format of static size.
	Wrapper of standard array of UPos.
	@tparam N Format size
     */
    template <SizeT N>
    class MFormat
    {
    public:
	typedef Arr<UPos,N> InputType;
	
	SP_DEFAULT_MEMBERS(constexpr,MFormat); /**< default constructors and assignments */

	/** Construct MFormat from standard array.
	    @param b Input array
	 */
	explicit constexpr MFormat(const Arr<UPos,N>& b);

	/** Construct MFormat from format of arbitrary type.
	    The input format size has to match the static size N.
	    @param f Input format
	 */
	template <class FormatT>
	constexpr MFormat(const FormatT& f);

	/** Get underlying array. */
	const Arr<UPos,N>& all() const;
	
	/** Get format size. */
	constexpr decltype(auto) size() const;
	
	/** Get format element.
	    @param i CSizeT indicating static element position 
	 */
	template <SizeT I>
	constexpr decltype(auto) get(CSizeT<I> i) const;

	/** Get format element.
	    @param i CSizeT indicating static element position 
	 */
	template <SizeT I>
	constexpr decltype(auto) operator[](CSizeT<I> i) const;

    private:
	Arr<UPos,N> mB;

    };

    /** ****
	MFormat has static size.
	@see is_static_format
     */
    template <SizeT N> struct is_static_format<MFormat<N>> { CXZ_CVAL_TRUE; };

    /** ****
	Multi index format of static size.
	Wrapper of standard tuple of position types.
	@tparam PosT Position types.
     */
    template <class... PosT>
    class GMFormat
    {
    public:
	typedef Tuple<PosT...> InputType;

	SP_DEFAULT_MEMBERS(constexpr,GMFormat); /**< default constructors and assignments */

	/** Construct from tuple.
	    @param b Input tuple.
	 */
	explicit constexpr GMFormat(const Tuple<PosT...>& b);

	/** Construct from tuple (move).
	    @param b Input tuple.
	 */
	explicit constexpr GMFormat(Tuple<PosT...>&& b);

	/** Construct MFormat from format of arbitrary type.
	    The input format size has to match the number of entries
	    and the input entries have to be compatible with the position types.
	    @param f Input format
	 */
	template <class FormatT>
	constexpr GMFormat(const FormatT& f);

	/** Get underlying tuple. */
	const Tuple<PosT...>& all() const;
	
	/** Get format size. */
	constexpr decltype(auto) size() const;

	/** Get format element.
	    @param i CSizeT indicating static element position 
	 */
	template <SizeT I>
	constexpr decltype(auto) get(CSizeT<I> i) const;

	/** Get format element.
	    @param i CSizeT indicating static element position 
	 */
	template <SizeT I>
	constexpr decltype(auto) operator[](CSizeT<I> i) const;
	
    private:
	Tuple<PosT...> mB;
    };

    /** Create GMFormat from position types.
	@param ps Position types.
     */
    template <class... PosT>
    constexpr decltype(auto) gmformat(const PosT&... ps);
    
    /** ****
	GMFormat has static size.
	@see is_static_format
     */
    template <class... PosT> struct is_static_format<GMFormat<PosT...>> { CXZ_CVAL_TRUE; };

    /** ****
	Multi index format of variable size
	Wrapper of standard vector of UPos.
     */
    class YFormat
    {
    public:
	typedef Vector<UPos> InputType;
	
	DEFAULT_MEMBERS(YFormat); /**< default constructors and assignments */

	/** Construct from vector.
	    @param b Input vector.
	 */
	explicit YFormat(const Vector<UPos>& b);

	/** Construct from format of arbitrary type.
	    @param f Input format.
	 */
	template <class FormatT>
	YFormat(const FormatT& f);

	/** Get underlying vector. */
	const Vector<UPos>& all() const;

	/** Get format size. */
	SizeT size() const;

	/** Get format element.
	    @param i CSizeT indicating static element position 
	 */
	template <SizeT I>
	const UPos& get(CSizeT<I> i) const;
	
	/** Get format element.
	    @param i CSizeT indicating static element position 
	 */
	template <SizeT I>
	const UPos& operator[](CSizeT<I> i) const;

	/** Get format element.
	    @param i E element position 
	 */
	const UPos& get(SizeT i) const;

	/** Get format element.
	    @param i E element position 
	 */
	const UPos& operator[](SizeT i) const;
	
    private:
	Vector<UPos> mB;
    };

    /** Check if format is trivial, i.e. f[i-1] = f[i]*s[i].
	@param f Vector representing the index format.
	@param s Vector representing the sub-index maxima.
     */
    bool formatIsTrivial(const Vector<SizeT>& f, const Vector<SizeT>& s);
}

#endif
