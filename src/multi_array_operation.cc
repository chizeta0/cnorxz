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
    OperationTemplate<OperationClass>::OperationTemplate(OperationClass* oc) : mOc(oc) {}
    
    template <class OperationClass>
    template <class Second>
    Operation<double,std::plus<double>,OperationClass,Second>
    OperationTemplate<OperationClass>::operator+(const Second& in) const
    {
    	return Operation<double,std::plus<double>,OperationClass,Second>(*mOc, in);
    }
    
    /*************************
     *   OperationMaster     *
     *************************/

    template <typename T, class... Ranges>
    OperationMaster<T,Ranges...>::
    OperationMaster(MutableMultiArrayBase<T,CRange>& ma, const OperationBase<T>& second,
		    std::shared_ptr<typename CRange::IndexType>& index) :
	mSecond(second), mArrayRef(ma), mIndex()
    {
	MultiRangeFactory<Ranges...> mrf( index->range() );
	mIndex = mrf.create();	
	CHECK;
	(*mIndex) = *index;
	CHECK;
	for(*mIndex = 0; mIndex->pos() != mIndex->max(); ++(*mIndex)){
	    get() = mSecond.get();
	}
	CHECK;
    }
    
    template <typename T, class... Ranges>
    T& OperationMaster<T,Ranges...>::get()
    {
	return mArrayRef.data()[ mIndex->pos() ];
    }
    
    template <typename T, class... Ranges>
    const T& OperationMaster<T,Ranges...>::get() const
    {
	return mArrayRef.data()[ mIndex->pos() ];
    }
    
    /****************************
     *   ConstOperationRoot     *
     ****************************/

    template <typename T, class... Ranges>
    ConstOperationRoot<T,Ranges...>::
    ConstOperationRoot(const MultiArrayBase<T,CRange>& ma,
		       const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	OperationBase<T>(), OperationTemplate<ConstOperationRoot<T,Ranges...> >(this),
	mArrayRef(ma), mIndex( std::make_shared<IndexType>( mArrayRef.range() ) )
    {
	CHECK;
	mIndex(indices...);
	CHECK;
    }

    template <typename T, class... Ranges>
    const T& ConstOperationRoot<T,Ranges...>::get() const
    {
	return mArrayRef[ *mIndex ];
    }
    
    /***********************
     *   OperationRoot     *
     ***********************/

    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>::
    OperationRoot(MutableMultiArrayBase<T,CRange>& ma,
		  const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	MutableOperationBase<T>(), OperationTemplate<OperationRoot<T,Ranges...> >(this),
	mArrayRef(ma), mIndex( std::make_shared<IndexType>( mArrayRef.range() ) )
    {
	CHECK;
	(*mIndex)(indices...);
	CHECK;
    }

    template <typename T, class... Ranges>
    OperationMaster<T,Ranges...> OperationRoot<T,Ranges...>::operator=(const OperationBase<T>& in)
    {
	return OperationMaster<T,Ranges...>(mArrayRef, in, mIndex);
    }
    
    template <typename T, class... Ranges>
    const T& OperationRoot<T,Ranges...>::get() const
    {
	return mArrayRef[ *mIndex ];
    }

    template <typename T, class... Ranges>
    T& OperationRoot<T,Ranges...>::get()
    {
	return mArrayRef[ *mIndex ];
    }

    /***********************
     *   OperationRoot     *
     ***********************/
    
    template <typename T, class OpFunction, class... Ops>
    Operation<T,OpFunction,Ops...>::Operation(const Ops&... ops) :
	OperationBase<T>(), OperationTemplate<Operation<T,OpFunction,Ops...> >(this),
	mOps(ops...) {}

    template <typename T, class OpFunction, class... Ops>
    const T& Operation<T,OpFunction,Ops...>::get() const
    {
	mRes = PackNum<sizeof...(Ops)-1>::template unpackArgs<T,OpFunction>(mOps);
	return mRes;
    }
}
