
#ifndef __cxz_array_base_cc_h__
#define __cxz_array_base_cc_h__

#include "array_base.h"

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
	//assertCompatible(i,beg);
	// check further compatibility of index/range format!!!
	auto ai = beg + i.lex();
	return *ai;
    }

    template <typename T>
    template <typename I, typename M>
    Sptr<CArrayBase<T>> CArrayBase<T>::sl(const IndexInterface<I,M>& i) const
    {
	////auto r = mkSliceRange(i.range());
	//auto beg = this->begin();
	//assertCompatible(i,beg);
	////auto bs = mkSliceBlockSizes(i, beg);
	//auto it = beg + i.lex();
	////return std::make_shared<CSlice>(r, this, bs, it.pos());
	return std::make_shared<CSlice>();
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
    
    /*
    template <typename T>
    template <typename I, typename M>
    ConstOperationRoot<T,I> CArrayBase<T>::operator()(const IndexPtr<I,M>& i) const
    {
	return ConstOperationRoot<T,I>();
    }
    */

    /******************************
     *   CArrayBase (protected)   *
     ******************************/

    template <typename T>
    template <class IPack1, class IPack2>
    inline Vector<SizeT> CArrayBase<T>::mkSliceBlockSize(const IPack1& ip1, const IPack2& ip2) const
    {
	const SizeT ip1dim = indexPackDim(ip1);
	const SizeT ip2dim = indexPackDim(ip2);
	if(ip1dim > ip2dim){
	    //const SizeT ip1sdim = indexPackSDim(ip1);
	    const SizeT ip2sdim = indexPackSDim(ip2);
	    CXZ_ASSERT(ip1dim == ip2sdim or ip2sdim == ip2dim,
		       "")
	}
	else if(ip1dim < ip2dim){

	}
	else {

	}
    }

    template <typename T>
    inline RangePtr CArrayBase<T>::mkSliceRange(const RangePtr& r) const
    {
	
    }

    template <typename T>
    inline void CArrayBase<T>::assertCompatible() const
    {
	
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
	auto r = mkSliceRange(i);
	auto beg = this->begin();
	auto bs = mkSliceBlockSizes(i, beg);
	auto it = beg + i.lex();
	return std::make_shared<Slice>(r, this, bs, it.pos());
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

    /*
    template <typename T>
    template <typename I, typename M>
    OperationRoot<T,I> ArrayBase<T>::operator()(const IndexPtr<I,M>& i)
    {
	return OperationRoot<T,I>();
    }
    */
}

#endif
