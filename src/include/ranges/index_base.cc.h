// -*- C++ -*-
/**
   
   @file include/ranges/index_base.cc.h
   @brief Index base template implementation.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_index_base_cc_h__
#define __cxz_index_base_cc_h__

#include "index_base.h"

namespace CNORXZ
{
    /*====================+
     |   IndexInterface   |
     +====================*/

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>::IndexInterface()
    {
	mPtrId = reinterpret_cast<PtrId>(this);
    }

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>::IndexInterface(const IndexInterface& in) :
	mPos(in.mPos)
    {
	mPtrId = reinterpret_cast<PtrId>(this);
    }

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>::IndexInterface(IndexInterface&& in) :
	mPos(in.mPos)
    {
	mPtrId = reinterpret_cast<PtrId>(this);
    }

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>& IndexInterface<I,MetaType>::operator=(const IndexInterface& in)
    {
	mPos = in.mPos;
	return *this;
    }

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>& IndexInterface<I,MetaType>::operator=(IndexInterface&& in)
    {
	mPos = in.mPos;
	return *this;
    }
    
    template <class I, typename MetaType>
    IndexInterface<I,MetaType>::IndexInterface(SizeT pos) :
	mPos(pos)
    {
	mPtrId = reinterpret_cast<PtrId>(this);
    }

    template <class I, typename MetaType>
    bool IndexInterface<I,MetaType>::operator==(const IndexInterface& in) const
    {
	return in.lex() == lex() and *in.range() == *range();
    }

    template <class I, typename MetaType>
    bool IndexInterface<I,MetaType>::operator!=(const IndexInterface& in) const
    {
	return in.lex() != lex() or *in.range() != *range();
    }

    template <class I, typename MetaType>
    bool IndexInterface<I,MetaType>::operator<(const IndexInterface& in) const
    {
	return lex() < in.lex();
    }

    template <class I, typename MetaType>
    bool IndexInterface<I,MetaType>::operator>(const IndexInterface& in) const
    {
	return lex() > in.lex();
    }

    template <class I, typename MetaType>
    bool IndexInterface<I,MetaType>::operator<=(const IndexInterface& in) const
    {
	return lex() <= in.lex();
    }

    template <class I, typename MetaType>
    bool IndexInterface<I,MetaType>::operator>=(const IndexInterface& in) const
    {
	return lex() >= in.lex();
    }

    template <class I, typename MetaType>
    SizeT IndexInterface<I,MetaType>::pos() const
    {
	return mPos;
    }

    template <class I, typename MetaType>
    PtrId IndexInterface<I,MetaType>::ptrId() const
    {
	// if this assert never applies, remove mPtrId (-> Defaults) !!!
	assert(mPtrId == reinterpret_cast<PtrId>(this));
	return mPtrId;
    }


    /*==========================+
     |   Non-member functions   |
     +==========================*/
    
    template <class I, typename MetaType>
    IndexPtr<I,MetaType>& operator++(const IndexPtr<I,MetaType>& i)
    {
	++(*i);
	return i;
    }

    template <class I, typename MetaType>
    IndexPtr<I,MetaType>& operator--(const IndexPtr<I,MetaType>& i)
    {
	--(*i);
	return i;
    }

    template <class I, typename MetaType>
    IndexPtr<I,MetaType> operator+(const IndexPtr<I,MetaType>& i, Int n)
    {
	return std::make_shared<IndexInterface<I,MetaType>>( *i + n );
    }

    template <class I, typename MetaType>
    IndexPtr<I,MetaType> operator-(const IndexPtr<I,MetaType>& i, Int n)
    {
	return std::make_shared<IndexInterface<I,MetaType>>( *i - n );
    }

    template <class I>
    Sptr<I> moveToPtr(I&& i)
    {
	return std::make_shared<I>(std::forward<I>(i));
    }

    template <class I>
    decltype(auto) xpr(const Sptr<I>& i)
    {
	static_assert(is_index<I>::value, "got non-index type");
	static_assert(index_expression_exists<I>::value, "expression for given index type does not exist");
	return i->xpr(i);
    }

}

#endif
