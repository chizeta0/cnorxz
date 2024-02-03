// -*- C++ -*-
/**
   
   @file include/ranges/index_pack.h
   @brief Index pack declarations.

   Index packs are sets of indices. In contrast to multi-indices like
   (G)MIndex or YIndex, there is no format that can be used to determine
   a linearized memory position. Hence, only a lexicographic position is well defined.
   Nevertheless, a function pos() is implemented, which just returns
   the same value as lex().

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

**/

#ifndef __cxz_index_pack_h__
#define __cxz_index_pack_h__

#include "base/base.h"
#include "xindex.h"

namespace CNORXZ
{
    /** ****
	Static index pack.
	Containes tuple of shared pointers to index instances.
	@tparam Indices Index types.
     */
    template <class... Indices>
    class SPack
    {
    public:
	SP_DEFAULT_MEMBERS(constexpr,SPack); /**< default constructors and assignments */

	/** Construct from index pointers
	    @param is index pointers
	 */
	constexpr SPack(const Sptr<Indices>&... is);

	/** Construct from index pointer tuple
	    @param is index pointer tuple
	 */
	constexpr SPack(const Tuple<Sptr<Indices>...>& is);

	/** get underlying index pointer tuple
	    @return underlying index pointer tuple
	 */
	constexpr const Tuple<Sptr<Indices>...>& all() const;

	/** get pack size i.e. dimension (non-recursive)
	    @return pack size
	 */
	constexpr SizeT size() const;

	/** get pack element
	    @return i'th element of the pack
	    @tparam I static position
	    @param i static position instance
	 */
	template <SizeT I>
	constexpr decltype(auto) get(CSizeT<I> i) const;

	/** get pack element
	    @return i'th element of the pack
	    @tparam I static position
	    @param i static position instance
	 */
	template <SizeT I>
	constexpr decltype(auto) operator[](CSizeT<I> i) const;

	/** Append index on the r.h.s. of the pack.
	    @tparam Index Type of the index to be appended
	    @param i index to be appended
	 */
	template <class Index>
	constexpr decltype(auto) rmul(const Sptr<Index>& i) const;

	/** Append index on the l.h.s. of the pack.
	    @tparam Index Type of the index to be appended
	    @param i Index to be appended
	 */
	template <class Index>
	constexpr decltype(auto) lmul(const Sptr<Index>& i) const;

	/** Append another index pack on the r.h.s. of this pack
	    @tparam Indices2 Types of the indices in the pack to be appended
	    @param p Index pack to be appended
	 */
	template <class... Indices2>
	constexpr decltype(auto) mul(const SPack<Indices2...>& p) const;

	/** create a range from the index pack
	    @return created range
	 */
	decltype(auto) mkRange() const;

	/** Get lexicographic position if pack was transformed to a (trivial) multi-index.
	    @return lexicographic position
	 */
	SizeT lex() const;

	/** Get lexicographic position if pack was transformed to a (trivial) multi-index.
	    @return lexicographic position
	 */
	SizeT pos() const;
	
    private:
	Tuple<Sptr<Indices>...> mIs;
    };

    /** Create static index pack
	@tparam Indices index types
	@param inds Indices to create the pack from
	@return Created pack
     */
    template <class... Indices>
    constexpr decltype(auto) spack(const Indices&... inds);

    /** Create static index pack from index pointers
	@tparam Indices index types
	@param inds Indices to create the pack from
	@return Created pack
     */
    template <class... Indices>
    constexpr decltype(auto) spackp(const Sptr<Indices>&... inds);

    /** ******
	Dynamic index pack.
	Containes array of shared pointers to XIndex instances.
     */
    class DPack
    {
    public:
	DEFAULT_MEMBERS(DPack); /**< default constructors and assignments */

	/** Create pack from XIndex instance
	    @param is Vector of XIndex pointers
	 */
	explicit DPack(const Vector<XIndexPtr>& is);

	/** Create pack from XIndex instance (move input vector)
	    @param is Vector of XIndex pointers
	 */
	explicit DPack(Vector<XIndexPtr>&& is);

	/** Create pack from a static pack (SPack)
	    @param p static pack used for construction
	 */
	template <class... Indices>
	explicit DPack(const SPack<Indices...>& p);

	/** get underlying index pointer vector
	    @return underlying index pointer vector
	 */
	const Vector<XIndexPtr>& all() const;

	/** get pack size i.e. dimension (non-recursive)
	    @return pack size
	 */
	SizeT size() const;

	/** get pack element
	    @return i'th element of the pack
	    @param i Position
	 */
	const XIndexPtr& get(SizeT i) const;

	/** get pack element
	    @return i'th element of the pack
	    @param i Position
	 */
	const XIndexPtr& operator[](SizeT i) const;

	/** Append index on the r.h.s.
	    @param i DIndex to be appended
	 */
	DPack rmul(const Sptr<DIndex>& i) const;

	/** Append index on the l.h.s.
	    @param i DIndex to be appended
	 */
	DPack lmul(const Sptr<DIndex>& i) const;

	/** Append pack on the r.h.s.
	    @param p Pack to be appnded
	 */
	DPack mul(const DPack& p) const;

	/** create a range from the index pack
	    @return created range
	 */
	RangePtr mkRange() const;

	/** Get lexicographic position if pack was transformed to a (trivial) multi-index.
	    @return lexicographic position
	 */
	SizeT lex() const;

	/** Get lexicographic position if pack was transformed to a (trivial) multi-index.
	    @return lexicographic position
	 */
	SizeT pos() const;
	
    private:
	Vector<XIndexPtr> mIs;
    };

    /** Create dynamic index pack
	@tparam Indices index types
	@param inds Indices to create the pack from
	@return Created pack
     */
    template <class... Indices>
    DPack dpack(const Indices&... inds);

    /** Create dynamic index pack from index pointers
	@tparam Indices index types
	@param inds Indices to create the pack from
	@return Created pack
     */
    template <class... Indices>
    DPack dpackp(const Sptr<Indices>&... inds);
}

#endif
