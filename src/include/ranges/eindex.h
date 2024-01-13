// -*- C++ -*-
/**
   
   @file include/ranges/eindex.h
   @brief ...


   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_eindex_h__
#define __cxz_eindex_h__

#include "base/base.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"
#include "xpr/xpr.h"

namespace CNORXZ
{
    /** *****
	Extension Index

	Elements accessed through this index in a loop
	are treated and processed through a single access
	allowing the usage of vector extensions
	
	@tparam MetaT Index meta data type
	@tparam S Vector size
	@tparam L Static index label
     */
    template <typename MetaT, SizeT S, SizeT L>
    class EIndex : public LIndex<SIndex<MetaT,S>,L>
    {
    public:
	typedef typename LIndex<SIndex<MetaT,S>,L>::IB IB;
	typedef typename LIndex<SIndex<MetaT,S>,L>::RangeType RangeType;

	DEFAULT_MEMBERS(EIndex); /**< default constructors and assignments */
	EIndex(const Sptr<LIndex<SIndex<MetaT,S>,L>>& i);

	/** @copydoc IndexInterface::ifor()

	    Specialization for EIndex: access all elements
	    at once, allowing usage of vector extensions
	    @see EFor
	 */
	template <class Xpr, class F>
	decltype(auto) ifor(const Xpr& xpr, F&& f) const;

    private:
	Sptr<LIndex<SIndex<MetaT,S>,L>> mLI;
    };

    /** ***
	EIndex is an index
	@see is_index
     */    
    template <typename MetaT, SizeT S, SizeT L>
    struct is_index<EIndex<MetaT,S,L>>
    {
	static constexpr bool value = true;
    };

    /** ***
	EIndex can be used as expression
	@see index_expression_exists
     */    
    template <typename MetaT, SizeT S, SizeT L>
    struct index_expression_exists<EIndex<MetaT,S,L>>
    {
	static constexpr bool value = true;
    };

    /** Make index pack from EIndex and second index of arbitrary type
     */    
    template <typename MetaT, SizeT S, SizeT L, class I1>
    decltype(auto) operator*(const Sptr<EIndex<MetaT,S,L>>& a, const Sptr<I1>& b);

    /** Create Eindex pointer from LIndex pointer
     */
    template <typename MetaT, SizeT S, SizeT L>
    decltype(auto) eindexPtr(const Sptr<LIndex<SIndex<MetaT,S>,L>>& i);

    /** Create Eindex pointer from SIndex pointer
	@tparam L Static index label
     */
    template <SizeT L, typename MetaT, SizeT S>
    decltype(auto) eindexPtr(const Sptr<SIndex<MetaT,S>>& i);

    /** Create Eindex pointer from LIndex pointer
	@param l Static index label
     */
    template <typename MetaT, SizeT S, SizeT L>
    decltype(auto) eindexPtr(const Sptr<SIndex<MetaT,S>>& i, CSizeT<L> l);

    /** Split given index into pack of EIndex and remainder index
	@param i Index to be split
	@tparam S Vector size
	@tparam L1 label of EIndex
	@tparam L2 label of remainder index
     */
    template <SizeT S, SizeT L1, SizeT L2, class Index>
    decltype(auto) eplex(const Sptr<Index>& i);

    /** Split given index into pack of EIndex and remainder index
	@param i Index to be split
	@param s Vector size
	@param l label of EIndex
     */
    template <class Index, SizeT S, SizeT L>
    decltype(auto) eplex(const Sptr<Index>& i, CSizeT<S> s, CSizeT<L> l);

    /** Split given index into pack of EIndex and remainder index
	@param i Index to be split
	@param s Vector size
	@param l1 label of EIndex
	@param l2 label of remainder index
     */
    template <class Index, SizeT S, SizeT L1, SizeT L2>
    decltype(auto) eplex(const Sptr<Index>& i, CSizeT<S> s, CSizeT<L1> l1, CSizeT<L2> l2);

}

#endif
