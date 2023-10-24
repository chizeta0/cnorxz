
#ifndef __cxz_slice_cc_h__
#define __cxz_slice_cc_h__

#include "slice.h"

namespace CNORXZ
{
    /**************
     *   CSlice   *
     **************/
    
    template <typename T>
    CSlice<T>::CSlice(const RangePtr& range, const CArrayBase<T>* parent,
		      const YFormat& blockSizes, SizeT off) :
	CArrayBase<T>(range),
	mCParent(parent),
	mBlockSizes(blockSizes),
	mOff(off)
    {}

    template <typename T>
    const T* CSlice<T>::data() const
    {
	return mCParent->data() + mOff;
    }
    
    template <typename T>
    typename CSlice<T>::const_iterator CSlice<T>::cbegin() const
    {
	return const_iterator(data(),YIndex(AB::mRange, mBlockSizes, 0));
    }
    
    template <typename T>
    typename CSlice<T>::const_iterator CSlice<T>::cend() const
    {
	return const_iterator(data(),YIndex(AB::mRange, mBlockSizes, AB::mRange->size()));
    }
    
    template <typename T>
    bool CSlice<T>::isView() const
    {
	return true;
    }

    /*************
     *   Slice   *
     *************/

    template <typename T>
    Slice<T>::Slice(const RangePtr& range, ArrayBase<T>* parent,
		    const YFormat& blockSizes, SizeT off) :
	ArrayBase<T>(range),
	mParent(parent),
	mBlockSizes(blockSizes),
	mOff(off)
    {}

    template <typename T>
    T* Slice<T>::data()
    {
	return mParent->data() + mOff;
    }

    template <typename T>
    const T* Slice<T>::data() const
    {
	return mParent->data() + mOff;
    }
    
    template <typename T>
    typename Slice<T>::const_iterator Slice<T>::cbegin() const
    {
	return const_iterator(data(),YIndex(AB::mRange, mBlockSizes, 0));
    }
    
    template <typename T>
    typename Slice<T>::const_iterator Slice<T>::cend() const
    {
	return const_iterator(data(),YIndex(AB::mRange, mBlockSizes, AB::mRange->size()));
    }
    
    template <typename T>
    bool Slice<T>::isView() const
    {
	return true;
    }
}

#endif
