
#ifndef __cxz_darray_cc_h__
#define __cxz_darray_cc_h__

#include "darray.h"

namespace CNORXZ
{
    /****************
     *    DArray    *
     ***************/
    
    template <typename T>
    DArray<T>::DArray(const RangePtr& range) :
	MDArrayBase<T>(range), mCont(range->size())
    {}

    template <typename T>
    DArray<T>::DArray(const RangePtr& range, const Vector<T>& vec) :
	MDArrayBase<T>(range), mCont(vec)
    {}

    template <typename T>
    DArray<T>::DArray(const RangePtr& range, Vector<T>&& vec) :
	MDArrayBase<T>(range), mCont(vec)
    {}

    template <typename T>
    SizeT DArray<T>::pmax() const
    {
	return mCont.size();
    }
    
    template <typename T>
    const T* DArray<T>::data() const
    {
	return mCont.data();
    }

    template <typename T>
    T* DArray<T>::data()
    {
	return mCont.data();
    }

    template <typename T>
    typename DArray<T>::const_iterator DArray<T>::cbegin() const
    {
	return const_iterator(mCont.data(), AB::mRange);
    }

    template <typename T>
    typename DArray<T>::const_iterator DArray<T>::cend() const
    {
	return const_iterator(mCont.data(), AB::mRange, mCont.size());
    }

    template <typename T>
    bool DArray<T>::isView() const
    {
	return false;
    }
    
}

#endif
