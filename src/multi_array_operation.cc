// -*- C++ -*-

#include "multi_array_operation.h"

namespace MultiArrayTools
{

       
    /*********************************
     *   MultiArrayOperationBase     *
     *********************************/
    
    // purely virtual at the moment


    /***************************
     *   OperationTemplate     *
     ***************************/

    template <class OperationClass>
    template <class Second>
    Operation<OperationClass,Second> OperationTemplate<OperationClass>::operator+(Second&& in) const
    {
	return Operation<double,std::plus<double>,OperationClass,Second>(*this, in);
    }
    
    /*************************
     *   OperationMaster     *
     *************************/

    template <typename T, class... Ranges>
    OperationMaster<T,Ranges...>::
    OperationMaster(MutableMultiArrayBase& ma,
		    std::shared_ptr<ContainerRange<Ranges...>::IndexType>& index) :
	mArrayRef(ma), mIndex()
    {
	(*mIndex) = *index;
	for(*mIndex = 0; mIndex->pos() != mIndex->max(); ++(*mIndex)){
	    get() = mSecond.get();
	}
    }
    
    template <typename T, class... Ranges>
    T& OperationMaster<T,Ranges...>::get()
    {
	return mArrayRef.data()[ mIndex.pos() ];
    }
    
    template <typename T, class... Ranges>
    const T& OperationMaster<T,Ranges...>::get() const
    {
	return mArrayRef.data()[ mIndex.pos() ];
    }
    
    /****************************
     *   ConstOperationRoot     *
     ****************************/

    template <typename T, class... Ranges>
    ConstOperationRoot<T,CRange,Ranges...>::
    ConstOperationRoot(const MultiArrayBase<T,CRange>& ma,
		       const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	mArrayRef(ma), mIndex(mArrayRef)
    {
	mIndex(indices...);
    }

    template <typename T, class... Ranges>
    const T& ConstOperationRoot<T,Ranges...>::get() const
    {
	return mArrayRef[ mIndex ];
    }
    
    /***********************
     *   OperationRoot     *
     ***********************/

    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>::
    OperationRoot(MutableMultiArrayBase<T,CRange>& ma,
		  const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	mArrayRef(ma), mIndex(mArrayRef)
    {
	mIndex(indices...);
    }

    template <typename T, class... Ranges>
    OperationMaster<T,Ranges...> OperationRoot<T,Ranges...>::operator=(const OperationBase<T>& in)
    {
	return OperationMaster<T,Ranges...>(mArrayRef, in, mIndex);
    }
    
    template <typename T, class... Ranges>
    const T& OperationRoot<T,Ranges...>::get() const
    {
	return mArrayRef[ mIndex ];
    }

    template <typename T, class... Ranges>
    T& OperationRoot<T,Ranges...>::get()
    {
	return mArrayRef[ mIndex ];
    }

    /***********************
     *   OperationRoot     *
     ***********************/

    template <typename T, class OpFunction, class... Ops>
    Operation<T,OpFunction,Ops...>::Operation(Ops&&... ops) : mOps(ops...) {}

    template <typename T, class OpFunction, class... Ops>
    const T& Operation<T,OpFunction,Ops...>::get() const
    {
	mRes = F(mOps);
	return mRes;
    }
}
