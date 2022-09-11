
#include "cxz_darray.h"

namespace CNORXZ
{
    /****************
     *    DArray    *
     ***************/
    
    template <typename T>
    DArray<T>::DArray(const RangePtr& range) : MDArrayBase<T>(range), mCont(range->size()) {}

    template <typename T>
    DArray<T>::DArray(const RangePtr& range, const vector<T>& vec) :
	MDArrayBase<T>(range), mCont(vec) {}

    template <typename T>
    DArray<T>::DArray(const RangePtr& range, vector<T>&& vec) :
	MDArrayBase<T>(range), mCont(vec) {}

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
	assert(0);
	return const_iterator();
    }

    template <typename T>
    typename DArray<T>::const_iterator DArray<T>::cend() const
    {
	assert(0);
	return const_iterator();
    }

    template <typename T>
    bool DArray<T>::isView() const
    {
	return false;
    }
    
}
