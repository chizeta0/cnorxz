
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
	auto ai = itLex(i);
	return *ai;
    }
    
    template <typename T>
    template <typename I, typename M>
    const T& CArrayBase<T>::at(const IndexInterface<I,M>& i) const
    {
	auto ai = itLexSave(i);
	return *ai;
    }

    template <typename T>
    template <class... Indices>
    const T& CArrayBase<T>::operator[](const SPack<Indices...>& pack) const
    {
	auto ai = itLex(pack);
	return *ai;
    }
    
    template <typename T>
    template <class... Indices>
    const T& CArrayBase<T>::at(const SPack<Indices...>& pack) const
    {
	auto ai = itLexSave(pack);
	return *ai;
    }

    template <typename T>
    const T& CArrayBase<T>::operator[](const DPack& pack) const
    {
	auto ai = itLex(pack);
	return *ai;
    }
    
    template <typename T>
    const T& CArrayBase<T>::at(const DPack& pack) const
    {
	auto ai = itLexSave(pack);
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
    template <class Index>
    COpRoot<T,Index> CArrayBase<T>::operator()(const Sptr<Index>& i) const
    {
	CXZ_WARNING("FORMAT / BLOCKSIZES!!!");
	return coproot(*this, i);
    }

    template <typename T>
    template <class... Indices>
    inline decltype(auto) CArrayBase<T>::operator()(const SPack<Indices...>& pack) const
    {
	CXZ_WARNING("FORMAT / BLOCKSIZES!!!");
	return coproot(*this, mindexPtr(pack));
    }

    template <typename T>
    inline decltype(auto) CArrayBase<T>::operator()(const DPack& pack) const
    {
	CXZ_WARNING("FORMAT / BLOCKSIZES!!!");
	return coproot(*this, yindexPtr(pack));
    }

    /******************************
     *   CArrayBase (protected)   *
     ******************************/

    template <typename T>
    template <class Acc>
    typename CArrayBase<T>::const_iterator CArrayBase<T>::itLex(const Acc& acc) const
    {
	return begin() + acc.lex();
    }
    
    template <typename T>
    template <class Acc>
    typename CArrayBase<T>::const_iterator CArrayBase<T>::itLexSave(const Acc& acc) const
    {
	CXZ_ASSERT(acc.lex() < this->size(), "index out of range");
	//CXZ_ASSERT(false, "IMPLEMENT CHECKS!!");
	// check further compatibility of index/range format!!!
	return begin() + acc.lex();
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
	auto ai = itLex(i);
	return *ai;
    }
    
    template <typename T>
    template <typename I, typename M>
    T& ArrayBase<T>::at(const IndexInterface<I,M>& i)
    {
	auto ai = itLexSave(i);
	return *ai;
    }

    template <typename T>
    template <class... Indices>
    T& ArrayBase<T>::operator[](const SPack<Indices...>& pack)
    {
	auto ai = itLex(pack);
	return *ai;
    }
    
    template <typename T>
    template <class... Indices>
    T& ArrayBase<T>::at(const SPack<Indices...>& pack)
    {
	auto ai = itLexSave(pack);
	return *ai;
    }

    template <typename T>
    T& ArrayBase<T>::operator[](const DPack& pack)
    {
	auto ai = itLex(pack);
	return *ai;
    }
    
    template <typename T>
    T& ArrayBase<T>::at(const DPack& pack)
    {
	auto ai = itLexSave(pack);
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
    template <class Index>
    OpRoot<T,Index> ArrayBase<T>::operator()(const Sptr<Index>& i)
    {
	CXZ_WARNING("FORMAT / BLOCKSIZES!!!");
	return oproot(*this, i);
    }

    template <typename T>
    template <class... Indices>
    inline decltype(auto) ArrayBase<T>::operator()(const SPack<Indices...>& pack)
    {
	CXZ_WARNING("FORMAT / BLOCKSIZES!!!");
	return oproot(*this, mindexPtr(pack));
    }

    template <typename T>
    inline decltype(auto) ArrayBase<T>::operator()(const DPack& pack)
    {
	CXZ_WARNING("FORMAT / BLOCKSIZES!!!");
	return oproot(*this, yindexPtr(pack));
    }

    /*****************************
     *   ArrayBase (protected)   *
     *****************************/

    template <typename T>
    template <class Acc>
    typename ArrayBase<T>::iterator ArrayBase<T>::itLex(const Acc& acc)
    {
	return begin() + acc.lex();
    }
    
    template <typename T>
    template <class Acc>
    typename ArrayBase<T>::iterator ArrayBase<T>::itLexSave(const Acc& acc)
    {
	CXZ_ASSERT(acc.lex() < this->size(), "index out of range");
	//CXZ_ASSERT(false, "IMPLEMENT CHECKS!!");
	// check further compatibility of index/range format!!!
	return begin() + acc.lex();
    }

}

#endif