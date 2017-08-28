// -*- C++ -*-

#include "multi_array_operation.h"

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }

    void seekIndexInst(std::shared_ptr<const IndexBase> i, std::vector<std::shared_ptr<const IndexBase> >& ivec)
    {
	for(size_t inum = 0; inum != i->range()->dim(); ++inum){
	    auto ii = i->getPtr(inum);
	    if(ii->type() == IndexType::MULTI){
		seekIndexInst(ii, ivec);
	    }
	    ivec.push_back(ii);
	}
    }
    
    BlockType getBlockType(std::shared_ptr<const IndexBase> i,
			   std::shared_ptr<const IndexBase> j, bool first)
    {
	BlockType out = BlockType::VALUE;
	for(size_t inum = 0; inum != i->range()->dim(); ++inum){
	    if(ii == j){
		if(inum == 0){
		    out = BlockType::BLOCK;
		}
		else {
		    out = BlockType::SPLIT;
		}
		continue;
	    }
	    
	    if(ii->type() == IndexType::MULTI){		
		BlockType tmp = getBlockType(ii, j, ivec);
		if(tmp != BlockType::VALUE){
		    out = tmp;
		}
	    }
	}
	return out;
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

    template <typename T, class... Ranges>
    std::vector<BlockType> OperationMaster<T,Ranges...>::block(const std::shared_ptr<IndexBase>& blockIndex) const
    {
	// seek index with smallest number of SPLITs !!!
	
    }

    template <typename T, class... Ranges>
    OperationMaster<T,Ranges...>& OperationMaster<T,Ranges...>::block() const
    {
	mBlockPtr->set( &mArrayRef[ (*mIndex) ] );
	return *this;
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
	block();
	return *mBlockPtr;
    }

    template <typename T, class... Ranges>
    std::vector<BlockType> ConstOperationRoot<T,Ranges...>::block(const std::shared_ptr<IndexBase>& blockIndex) const
    {
	// !!!
    }

    template <typename T, class... Ranges>
    ConstOperationRoot<T,Ranges...>& ConstOperationRoot<T,Ranges...>::block() const
    {
	mBlockPtr->set( &mArrayRef[ (*mIndex)() ] );
	return *this;
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
	block();
	return *mBlockPtr;
    }

    template <typename T, class... Ranges>
    BlockBase<T>& OperationRoot<T,Ranges...>::get()
    {
	block();
	return *mBlockPtr; // issue: const !!!
    }

    template <typename T, class... Ranges>
    std::vector<BlockType> OperationRoot<T,Ranges...>::block(const std::shared_ptr<IndexBase>& blockIndex) const
    {
	// !!!
    }

    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>& OperationRoot<T,Ranges...>::block() const
    {
	mBlockPtr->set( &mArrayRef[ (*mIndex)() ] );
	return *this;
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

    template <typename T, class... Ranges>
    std::vector<BlockType> Operation<T,Ranges...>::block(const std::shared_ptr<IndexBase>& blockIndex) const
    {
	// !!!
    }

    template <typename T, class... Ranges>
    Operation<T,Ranges...>& Operation<T,Ranges...>::block() const
    {
	mBlockPtr->set( &mArrayRef[ (*mIndex)() ] );
	return *this;
    }

}
