// -*- C++ -*-

#include "multi_array_operation.h"

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }
       
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
    auto OperationTemplate<OperationClass>::operator+(const Second& in) const
	-> Operation<value_type,std::plus<value_type>,OperationClass,Second>
    {
    	return Operation<value_type,std::plus<value_type>,OperationClass,Second>(*mOc, in);
    }

    template <class OperationClass>
    template <class Second>
    auto OperationTemplate<OperationClass>::operator-(const Second& in) const
	-> Operation<value_type,std::minus<value_type>,OperationClass,Second>
    {
    	return Operation<value_type,std::minus<value_type>,OperationClass,Second>(*mOc, in);
    }
    
    template <class OperationClass>
    template <class Second>
    auto OperationTemplate<OperationClass>::operator*(const Second& in) const
	-> Operation<value_type,std::multiplies<value_type>,OperationClass,Second>
    {
    	return Operation<value_type,std::multiplies<value_type>,OperationClass,Second>(*mOc, in);
    }

    template <class OperationClass>
    template <class Second>
    auto OperationTemplate<OperationClass>::operator/(const Second& in) const
	-> Operation<double,std::divides<value_type>,OperationClass,Second>
    {
    	return Operation<value_type,std::divides<value_type>,OperationClass,Second>(*mOc, in);
    }

    /*************************
     *   OperationMaster     *
     *************************/

    template <typename T, class... Ranges>
    OperationMaster<T,Ranges...>::
    OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OperationBase<T>& second,
		    std::shared_ptr<typename CRange::IndexType>& index) :
	mSecond(second), mArrayRef(ma), mIndex()
    {
	MultiRangeFactory<Ranges...> mrf( index->range() );
	std::shared_ptr<MultiRange<Ranges...> > mr =
	    std::dynamic_pointer_cast<MultiRange<Ranges...> >( mrf.create() );
	mIndex = std::make_shared<IndexType>( mr->begin() );
	(*mIndex) = *index;
	for(*mIndex = 0; mIndex->pos() != mIndex->max(); ++(*mIndex)){
	    get() = mSecond.get();
	}
    }
    
    template <typename T, class... Ranges>
    BlockBase<T>& OperationMaster<T,Ranges...>::get()
    {
	return mArrayRef.data()[ mIndex->pos() ];
    }
    
    template <typename T, class... Ranges>
    const BlockBase<T>& OperationMaster<T,Ranges...>::get() const
    {
	return mArrayRef.data()[ mIndex->pos() ];
    }
    
    /****************************
     *   ConstOperationRoot     *
     ****************************/

    template <typename T, class... Ranges>
    ConstOperationRoot<T,Ranges...>::
    ConstOperationRoot(const MultiArrayBase<T,Ranges...>& ma,
		       const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	OperationBase<T>(), OperationTemplate<ConstOperationRoot<T,Ranges...> >(this),
	mArrayRef(ma), mIndex( std::make_shared<IndexType>( mArrayRef.range() ) )
    {
	mIndex(indices...);
    }

    template <typename T, class... Ranges>
    const BlockBase<T>& ConstOperationRoot<T,Ranges...>::get() const
    {
	return mArrayRef[ (*mIndex)() ];
    }
    
    /***********************
     *   OperationRoot     *
     ***********************/

    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>::
    OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		  const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	MutableOperationBase<T>(), OperationTemplate<OperationRoot<T,Ranges...> >(this),
	mArrayRef(ma), mIndex( std::make_shared<IndexType>( mArrayRef.range() ) )
    {
	(*mIndex)(indices...);
    }

    template <typename T, class... Ranges>
    OperationMaster<T,Ranges...> OperationRoot<T,Ranges...>::operator=(const OperationBase<T>& in)
    {
	return OperationMaster<T,Ranges...>(mArrayRef, in, mIndex);
    }
    
    template <typename T, class... Ranges>
    const BlockBase<T>& OperationRoot<T,Ranges...>::get() const
    {
	return mArrayRef[ (*mIndex)() ];
    }

    template <typename T, class... Ranges>
    BlockBase<T>& OperationRoot<T,Ranges...>::get()
    {
	return mArrayRef[ (*mIndex)() ];
    }

    /***********************
     *   OperationRoot     *
     ***********************/
    
    template <typename T, class OpFunction, class... Ops>
    Operation<T,OpFunction,Ops...>::Operation(const Ops&... ops) :
	OperationBase<T>(), OperationTemplate<Operation<T,OpFunction,Ops...> >(this),
	mOps(ops...) {}

    template <typename T, class OpFunction, class... Ops>
    const BlockBase<T>& Operation<T,OpFunction,Ops...>::get() const
    {
	mRes = PackNum<sizeof...(Ops)-1>::template unpackArgs<T,OpFunction>(mOps);
	return mRes;
    }
}
