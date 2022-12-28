
#ifndef __cxz_array_base_cc_h__
#define __cxz_array_base_cc_h__

#include "array_base.h"
#include "slice.h"

namespace CNORXZ
{
    /******************
     *   CArrayBase   *
     ******************/

    template <typename T>
    CArrayBase<T>::CArrayBase(const RangePtr& range) :
	mRange(rangeCast<YRange>(range))
    {}

    template <typename T>
    template <typename I, typename M>
    const T& CArrayBase<T>::operator[](const IndexInterface<I,M>& i) const
    {
	auto ai = this->begin() + i.lex();
	return *ai;
    }
    
    template <typename T>
    template <typename I, typename M>
    const T& CArrayBase<T>::at(const IndexInterface<I,M>& i) const
    {
	CXZ_ASSERT(i.lex() < this->size(), "index out of range");
	auto beg = this->begin();
	//CXZ_ASSERT(false, "IMPLEMENT CHECKS!!");
	// check further compatibility of index/range format!!!
	auto ai = beg + i.lex();
	return *ai;
    }

    template <typename T>
    template <typename I, typename M>
    Sptr<CArrayBase<T>> CArrayBase<T>::sl(const IndexInterface<I,M>& i) const
    {
	auto beg = std::make_shared<const_iterator>(this->begin());
	auto si = i.slice(beg);
	auto it = *beg + i.lex();
	return std::make_shared<CSlice>(this, *si, it.pos());
    }

    template <typename T>
    SizeT CArrayBase<T>::size() const
    {
	return mRange->size();
    }
    
    template <typename T>
    RangePtr CArrayBase<T>::range() const
    {
	return mRange;
    }

    template <typename T>
    typename CArrayBase<T>::const_iterator CArrayBase<T>::begin() const
    {
	return this->cbegin();
    }
    
    template <typename T>
    typename CArrayBase<T>::const_iterator CArrayBase<T>::end() const
    {
	return this->cend();
    }
    
    template <typename T>
    template <class I, typename M>
    COpRoot<T,I> CArrayBase<T>::operator()(const IndexPtr<I,M>& i) const
    {
	auto fi = this->cbegin().format( std::static_pointer_cast<I>(i) );
	return COpRoot<T,I>(*this, fi);
    }

    template <typename T>
    template <class I, SizeT L>
    COpRoot<T,I> CArrayBase<T>::operator()(const Sptr<LIndex<I,L>>& i) const
    {
	auto fi = this->cbegin().format( i );
	return COpRoot<T,LIndex<I,L>>(*this, fi);
    }

    /*****************
     *   ArrayBase   *
     *****************/

    template <typename T>
    ArrayBase<T>::ArrayBase(const RangePtr& range) :
	CArrayBase<T>(range)
    {}

    template <typename T>
    template <typename I, typename M>
    T& ArrayBase<T>::operator[](const IndexInterface<I,M>& i)
    {
	auto ai = this->begin() + i.lex();
	return *ai;
    }

    template <typename T>
    template <typename I, typename M>
    T& ArrayBase<T>::at(const IndexInterface<I,M>& i)
    {
	CXZ_ASSERT(i.les() < this->size(), "index out of range");
	// check further compatibility of index/range format!!!
	auto ai = this->begin() + i.lex();
	return *ai;
    }

    template <typename T>
    template <typename I, typename M>
    Sptr<ArrayBase<T>> ArrayBase<T>::sl(const IndexInterface<I,M>& i)
    {
	auto beg = std::make_shared<const_iterator>(this->cbegin());
	auto si = i.slice(beg);
	auto it = *beg + i.lex();
	return std::make_shared<Slice>(this, *si, it.pos());
    }

    template <typename T>
    typename ArrayBase<T>::iterator ArrayBase<T>::begin()
    {
	return iterator(this->data(), this->cbegin());
    }

    template <typename T>
    typename ArrayBase<T>::iterator ArrayBase<T>::end()
    {
	return iterator(this->data(), this->cend());
    }

    template <typename T>
    template <class I, typename M>
    OpRoot<T,I> ArrayBase<T>::operator()(const IndexPtr<I,M>& i)
    {
	auto fi = this->cbegin().format( std::static_pointer_cast<I>(i) );
	return OpRoot<T,I>(*this, fi);
    }

    template <typename T>
    template <class I, SizeT L>
    OpRoot<T,I> ArrayBase<T>::operator()(const Sptr<LIndex<I,L>>& i)
    {
	auto fi = this->cbegin().format( i );
	return OpRoot<T,LIndex<I,L>>(*this, fi);
    }

}

#endif
