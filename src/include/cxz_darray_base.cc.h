
#include "cxz_darray_base.h"

namespace CNORXZ
{
    /****************
     *  DArrayBase  *
     ***************/

    template <typename T>
    DArrayBase<T>::DArrayBase(const RangePtr& range) : mRange(range), mInit(true)
    {
	mIt = this->begin();
    }

    template <typename T>
    template <typename I>
    const value_type& DArrayBase<T>::operator[](const IndexInterface<I>& i) const
    {
	return *mIt(i);
    }
    
    template <typename T>
    template <typename I>
    const value_type& DArrayBase<T>::at(const IndexInterface<I>& i) const
    {
	return *mIt.at(i);
    }

    template <typename T>
    size_t DArrayBase<T>::size() const
    {
	return mRange->size();
    }
    
    template <typename T>
    RangePtr DArrayBase<T>::range() const
    {
	return mRange;
    }

    template <typename T>
    DArrayBase<T>::const_iterator DArrayBase<T>::begin() const
    {
	return this->cbegin();
    }
    
    template <typename T>
    DArrayBase<T>::const_iterator DArrayBase<T>::end() const
    {
	return this->cend();
    }
    
    template <typename T>
    virtual bool DArrayBase<T>::isInit() const
    {
	return mInit;
    }

    template <typename T>
    template <typename I>
    ConstOperationRoot<T,I> DArrayBase<T>::operator()(const IndexPtr<I>& i) const
    {
	return ConstOperationRoot<T,I>(/**/);
    }


    /*****************
     *  MDArrayBase  *
     ****************/

    template <typename T>
    MDArrayBase<T>::MDArrayBase(const RangePtr& range) : DArrayBase(range) {}

    template <typename T>
    template <typename I>
    value_type& MDArrayBase<T>::operator[](const IndexInterface<I>& i)
    {
	return *mIt(i);
    }

    template <typename T>
    template <typename I>
    value_type& MDArrayBase<T>::at(const IndexInterface<I>& i)
    {
	return *mIt.at(i);
    }

    template <typename T>
    MDArrayBase<T>::iterator MDArrayBase<T>::begin()
    {
	return iterator(this->data(), this->cbegin());
    }

    template <typename T>
    MDArrayBase<T>::iterator MDArrayBase<T>::end()
    {
	return iterator(this->data(), this->cend());
    }

    template <typename T>
    template <typename I>
    OperationRoot<T,I> MDArrayBase<T>::operator()(const IndexPtr<I>& i)
    {
	return OperationRoot<T,I>(/**/);
    }

}
