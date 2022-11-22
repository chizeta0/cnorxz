
#ifndef __cxz_darray_base_cc_h__
#define __cxz_darray_base_cc_h__

#include "darray_base.h"

namespace CNORXZ
{
    /******************
     *   DArrayBase   *
     ******************/

    template <typename T>
    DArrayBase<T>::DArrayBase(const RangePtr& range) :
	mRange(rangeCast<YRange>(range))
    {}

    template <typename T>
    template <typename I, typename M>
    const T& DArrayBase<T>::operator[](const IndexInterface<I,M>& i) const
    {
	auto ai = this->begin() + i.lex();
	return *ai;
    }
    
    template <typename T>
    template <typename I, typename M>
    const T& DArrayBase<T>::at(const IndexInterface<I,M>& i) const
    {
	CXZ_ASSERT(i.lex() < this->size(), "index out of range");
	// check further compatibility of index/range format!!!
	auto ai = this->begin() + i.lex();
	return *ai;
    }

    template <typename T>
    SizeT DArrayBase<T>::size() const
    {
	return mRange->size();
    }
    
    template <typename T>
    RangePtr DArrayBase<T>::range() const
    {
	return mRange;
    }

    template <typename T>
    typename DArrayBase<T>::const_iterator DArrayBase<T>::begin() const
    {
	return this->cbegin();
    }
    
    template <typename T>
    typename DArrayBase<T>::const_iterator DArrayBase<T>::end() const
    {
	return this->cend();
    }
    
    /*
    template <typename T>
    template <typename I, typename M>
    ConstOperationRoot<T,I> DArrayBase<T>::operator()(const IndexPtr<I,M>& i) const
    {
	return ConstOperationRoot<T,I>();
    }
    */

    /*******************
     *   MDArrayBase   *
     *******************/

    template <typename T>
    MDArrayBase<T>::MDArrayBase(const RangePtr& range) :
	DArrayBase<T>(range)
    {}

    template <typename T>
    template <typename I, typename M>
    T& MDArrayBase<T>::operator[](const IndexInterface<I,M>& i)
    {
	auto ai = this->begin() + i.lex();
	return *ai;
    }

    template <typename T>
    template <typename I, typename M>
    T& MDArrayBase<T>::at(const IndexInterface<I,M>& i)
    {
	CXZ_ASSERT(i.pos() < this->pmax(), "index out of range");
	// check further compatibility of index/range format!!!
	auto ai = this->begin() + i.lex();
	return *ai;
    }

    template <typename T>
    typename MDArrayBase<T>::iterator MDArrayBase<T>::begin()
    {
	return iterator(this->data(), this->cbegin());
    }

    template <typename T>
    typename MDArrayBase<T>::iterator MDArrayBase<T>::end()
    {
	return iterator(this->data(), this->cend());
    }

    /*
    template <typename T>
    template <typename I, typename M>
    OperationRoot<T,I> MDArrayBase<T>::operator()(const IndexPtr<I,M>& i)
    {
	return OperationRoot<T,I>();
    }
    */
}

#endif
