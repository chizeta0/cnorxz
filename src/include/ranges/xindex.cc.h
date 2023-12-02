// -*- C++ -*-
/**
   
   @file include/ranges/xindex.cc.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_xindex_cc_h__
#define __cxz_xindex_cc_h__

#include "xindex.h"

namespace CNORXZ
{

    /**************
     *   XIndex   *
     **************/
    
    template <class Index, typename Meta>
    XIndex<Index,Meta>::XIndex(const IndexPtr<Index,Meta>& i) :
	mI(i) {}

    template <class Index, typename Meta>
    XIndex<Index,Meta>::XIndex(const IndexInterface<Index,Meta>& i) :
	mI(std::make_shared<Index>(i.THIS())) {}

    template <class Index, typename Meta>
    XIndexPtr XIndex<Index,Meta>::copy() const
    {
	return std::make_shared<XIndex<Index,Meta>>(mI->THIS());
    }

    template <class Index, typename Meta>
    SizeT XIndex<Index,Meta>::pos() const
    {
	return mI->pos();
    }

    template <class Index, typename Meta>
    XIndex<Index,Meta>& XIndex<Index,Meta>::operator=(SizeT lexpos)
    {
	mI->THIS() = lexpos;
	assert(mI->lex() == lexpos);
	return *this;
    }

    template <class Index, typename Meta>
    XIndex<Index,Meta>& XIndex<Index,Meta>::operator++()
    {
	++(*mI);
	return *this;
    }

    template <class Index, typename Meta>
    XIndex<Index,Meta>& XIndex<Index,Meta>::operator--()
    {
	--(*mI);
	return *this;
    }

    template <class Index, typename Meta>
    Sptr<XIndexBase> XIndex<Index,Meta>::operator+(Int n) const
    {
	return std::make_shared<XIndex<Index,Meta>>(*mI + n);
    }
    
    template <class Index, typename Meta>
    Sptr<XIndexBase> XIndex<Index,Meta>::operator-(Int n) const
    {
	return std::make_shared<XIndex<Index,Meta>>(*mI - n);
    }
    
    template <class Index, typename Meta>
    SizeT XIndex<Index,Meta>::operator-(const XIndexBase& i) const
    {
	const XIndex<Index,Meta>* ip = dynamic_cast<const XIndex<Index,Meta>*>(&i);
	CXZ_ASSERT(ip != nullptr, "bad index type");
	return (*mI) - (*ip->mI);
    }
    
    template <class Index, typename Meta>
    XIndex<Index,Meta>& XIndex<Index,Meta>::operator+=(Int n)
    {
	(*mI) += n;
	return *this;
    }
    
    template <class Index, typename Meta>
    XIndex<Index,Meta>& XIndex<Index,Meta>::operator-=(Int n)
    {
	(*mI) -= n;
	return *this;
    }

    template <class Index, typename Meta>
    SizeT XIndex<Index,Meta>::lex() const
    {
	return mI->lex();
    }

    template <class Index, typename Meta>
    UPos XIndex<Index,Meta>::pmax() const
    {
	return UPos(mI->pmax());
    }

    template <class Index, typename Meta>
    UPos XIndex<Index,Meta>::lmax() const
    {
	return UPos(mI->lmax());
    }

    template <class Index, typename Meta>
    IndexId<0> XIndex<Index,Meta>::id() const
    {
	return IndexId<0>(mI->id().id());
    }
    
    template <class Index, typename Meta>
    DType XIndex<Index,Meta>::operator*() const
    {
	return DType(*(*mI));
    }

    template <class Index, typename Meta>
    SizeT XIndex<Index,Meta>::dim() const
    {
	return mI->dim();
    }

    template <class Index, typename Meta>
    RangePtr XIndex<Index,Meta>::range() const
    {
	return mI->range();
    }

    template <class Index, typename Meta>
    UPos XIndex<Index,Meta>::stepSize(const IndexId<0>& id) const
    {
	return mI->stepSize(id);
    }

    template <class Index, typename Meta>
    RangePtr XIndex<Index,Meta>::prange(const XIndexPtr& last) const
    {
	return mI->prange( std::dynamic_pointer_cast<XIndex<Index,Meta>>(last)->get() );
    }

    template <class Index, typename Meta>
    Vector<SizeT> XIndex<Index,Meta>::deepFormat() const
    {
	return toVec( mI->deepFormat() );
    }
    
    template <class Index, typename Meta>
    String XIndex<Index,Meta>::stringMeta() const
    {
	return mI->stringMeta();
    }

    template <class Index, typename Meta>
    DType XIndex<Index,Meta>::meta() const
    {
	return DType(mI->meta());
    }

    template <class Index, typename Meta>
    XIndexBase& XIndex<Index,Meta>::at(const DType& meta)
    {
	mI->at(std::any_cast<const Meta&>(meta.get()));
	return *this;
    }

    template <class Index, typename Meta>
    DXpr<None> XIndex<Index,Meta>::ifor(const DXpr<None>& xpr, NoF&& f) const
    {
	return DXpr<None>(mI->ifor(xpr, std::forward<NoF>(f)));
    }

    template <class Index, typename Meta>
    bool XIndex<Index,Meta>::formatIsTrivial() const
    {
	return mI->formatIsTrivial();
    }

    template <class Index, typename Meta>
    Index& XIndex<Index,Meta>::get()
    {
	return mI->THIS();
    }
    
    template <class Index, typename Meta>
    const Index& XIndex<Index,Meta>::get() const
    {
	return mI->THIS();
    }

    template <class Index>
    inline XIndexPtr xindexPtr(const Sptr<Index>& i)
    {
	typedef typename Index::MetaType Meta;
	return std::make_shared<XIndex<Index,Meta>>
	    (std::dynamic_pointer_cast<IndexInterface<Index,Meta>>(i));
    }

    template <>
    inline XIndexPtr xindexPtr<XIndexBase>(const Sptr<XIndexBase>& i)
    {
	return i;
    }
}

#endif
