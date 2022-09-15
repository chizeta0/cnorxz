
#ifndef __cxz_dcontainer_index_cc_h__
#define __cxz_dcontainer_index_cc_h__

#include "dcontainer_index.h"
#include "statics/static_for.h"

namespace CNORXZ
{

    template <typename T>
    DConstContainerIndex<T>::DConstContainerIndex(const T* data, const RangePtr& range):
	mI(range->begin()), mCData(data)
    {
	assert(0);
    }
	
    template <typename T>
    DConstContainerIndex<T>& DConstContainerIndex<T>::operator=(SizeT pos)
    {
	(*mI) = pos;
	IB::mPos = mI->pos();
	return *this;
    }

    template <typename T>
    DConstContainerIndex<T>& DConstContainerIndex<T>::operator++()
    {
	++(*mI);
	IB::mPos = mI->pos();
	return *this;
    }

    template <typename T>
    DConstContainerIndex<T>& DConstContainerIndex<T>::operator--()
    {
	--(*mI);
	IB::mPos = mI->pos();
	return *this;
    }

    template <typename T>
    int DConstContainerIndex<T>::pp(PtrId idxPtrNum)
    {
	return mI->pp(idxPtrNum);
    }

    template <typename T>
    int DConstContainerIndex<T>::mm(PtrId idxPtrNum)
    {
	return mI->mm(idxPtrNum);
    }

    template <typename T>
    SizeT DConstContainerIndex<T>::dim() const
    {
	return mI->dim();
    }

    template <typename T>
    SizeT DConstContainerIndex<T>::getStepSize(SizeT n) const
    {
	return mI->getStepSize(n); // dim() elements only!!!
    }

    template <typename T>
    String DConstContainerIndex<T>::stringMeta() const
    {
	return mI->stringMeta();
    }

    template <typename T>
    DType DConstContainerIndex<T>::meta() const
    {
	return mI->meta();
    }

    template <typename T>
    DConstContainerIndex<T>& DConstContainerIndex<T>::at(const DType& meta)
    {
	mI->at(meta);
	IB::mPos = mI->pos();
	return *this;
    }
    /*
    template <typename T>
    DExpr DConstContainerIndex<T>::ifor(SizeT step, DExpr ex) const
    {
	return mI->ifor(step, ex);
    }

    template <typename T>
    DExpr DConstContainerIndex<T>::iforh(SizeT step, DExpr ex) const
    {
	return mI->iforh(step, ex);
    }
    */
    template <typename T>
    const T& DConstContainerIndex<T>::operator*() const
    {
	//this->sync();
	return mCData[mI->pos()];
    }

    template <typename T>
    const T* DConstContainerIndex<T>::operator->() const
    {
	//this->sync();
	return mCData+mI->pos();
    }

    template <typename T>
    DContainerIndex<T>::DContainerIndex(T* data, const RangePtr& range) :
	DConstContainerIndex<T>(range), mData(data) {}

    template <typename T>
    DContainerIndex<T>::DContainerIndex(T* data, const DConstContainerIndex<T>& cci) :
	DConstContainerIndex<T>(data, cci.range()), mData(data) {}
    
    template <typename T>
    T& DContainerIndex<T>::operator*()
    {
	return mData[CCI::mI->pos()];
    }

    template <typename T>
    T* DContainerIndex<T>::operator->()
    {
	return mData+CCI::mI->pos();
    }

}

#endif
