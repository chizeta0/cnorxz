// -*- C++ -*-
/**
   
   @file include/ranges/lindex.h
   @brief Statically labeled index.

   Static index labels are usefull to resolve extensions and relations to
   other indices at compile time.
   
   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_lindex_h__
#define __cxz_lindex_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    /** ****
	Statically labeled index.
	@tparam Index Underlying index.
	@tparam L Static label.
     */
    template <class Index, SizeT L>
    class LIndex : public Index
    {
    public:
	typedef typename Index::IB IB;
	typedef typename Index::RangeType RangeType;

	DEFAULT_MEMBERS(LIndex); /**< Default constructors and assignments. */

	/** Construct from index pointer.
	    @param i Input index.
	 */
	LIndex(const Sptr<Index>& i);

	/** @copydoc IndexInterface::id()
	    Specialization: Static id equals L.
	 */
	IndexId<L> id() const;
	
	/** @copydoc IndexInterface::stepSize()
	    Specialization: stepSize may be static.
	 */
	template <SizeT I>
	decltype(auto) stepSize(const IndexId<I>& id) const;

	/** @copydoc IndexInterface::stepSize() */
	template <class Xpr, class F>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;

    private:
	Sptr<Index> mI;
    };

    /** ***
	LIndex is an index
	@see is_index
     */    
    template <class Index, SizeT L>
    struct is_index<LIndex<Index,L>>
    {
	static constexpr bool value = is_index<Index>::value; 
    };

    /** ***
	LIndex can be used as expression if this is case for its base type
	@see index_expression_exists
     */    
    template <class Index, SizeT L>
    struct index_expression_exists<LIndex<Index,L>>
    {
	static constexpr bool value = index_expression_exists<Index>::value;
    };

    /** Specialize index multiplication for LIndex.
	@param a Pointer to first index which is a LIndex.
	@param b Pointer to second index of arbitrary type.
	@return Resulting index pack.
     */
    template <class Index, SizeT L, class I1>
    decltype(auto) operator*(const Sptr<LIndex<Index,L>>& a, const Sptr<I1>& b);

    /** Create LIndex from index pointer.
	@param i Input index.
	@tparam L Static label.
	@return Resulting LIndex.
     */
    template <SizeT L, class Index>
    decltype(auto) lindexPtr(const Sptr<Index>& i);

    /** Create LIndex from index pointer.
	@param i Input index.
	@param l CSizeT indicating the static label.
	@return Resulting LIndex.
     */
    template <class Index, SizeT L>
    decltype(auto) lindexPtr(const Sptr<Index>& i, CSizeT<L> l);
}

#endif
