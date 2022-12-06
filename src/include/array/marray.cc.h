
#ifndef __cxz_marray_cc_h__
#define __cxz_marray_cc_h__

#include "marray.h"

namespace CNORXZ
{
    /****************
     *    MArray    *
     ***************/
    
    template <typename T>
    MArray<T>::MArray(const RangePtr& range) :
	ArrayBase<T>(range), mCont(range->size())
    {}

    template <typename T>
    MArray<T>::MArray(const RangePtr& range, const Vector<T>& vec) :
	ArrayBase<T>(range), mCont(vec)
    {}

    template <typename T>
    MArray<T>::MArray(const RangePtr& range, Vector<T>&& vec) :
	ArrayBase<T>(range), mCont(vec)
    {}

    template <typename T>
    SizeT MArray<T>::pmax() const
    {
	return mCont.size();
    }
    
    template <typename T>
    const T* MArray<T>::data() const
    {
	return mCont.data();
    }

    template <typename T>
    T* MArray<T>::data()
    {
	return mCont.data();
    }

    template <typename T>
    typename MArray<T>::const_iterator MArray<T>::cbegin() const
    {
	return const_iterator(mCont.data(), AB::mRange);
    }

    template <typename T>
    typename MArray<T>::const_iterator MArray<T>::cend() const
    {
	return const_iterator(mCont.data(), AB::mRange, mCont.size());
    }

    template <typename T>
    bool MArray<T>::isView() const
    {
	return false;
    }
    
}

#endif
