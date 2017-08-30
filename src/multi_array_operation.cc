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

    // !!!
    BTSS getBlockType(std::shared_ptr<const IndexBase> i,
					     std::shared_ptr<const IndexBase> j, bool first)
    {
	// returning BlockType and step size is redundant (change in the future)
	// stepSize == 0 => VALUE
	// stepSize == 1 => BLOCK
	// stepSize > 1 => SPLIT :)
	BTSS out(BlockType::VALUE, 0);
	for(size_t inum = 0; inum != i->range()->dim(); ++inum){
	    auto ii = i->getPtr(inum);
	    if(ii == j){
		if(inum == 0 and first){
		    out = BTSS(BlockType::BLOCK, 1);
		}
		else {
		    out = BTSS(BlockType::SPLIT, i->getStepSize(inum));
		}
		continue;
	    }
	    
	    if(ii->type() == IndexType::MULTI or
	       ii->type() == IndexType::CONT){	
		BTSS tmp = getBlockType(ii, j, inum == 0);
		if(tmp.first != BlockType::VALUE){
		    out = tmp;
		}
	    }
	}
	return out;
    }

    template <typename T>
    std::shared_ptr<BlockBase<T> > makeBlock(const std::vector<T>& vec, size_t stepSize, size_t blockSize)
    {
	if(stepSize == 0){
	    return std::make_shared<BlockValue<T> >(vec[0], blockSize);
	}
	else if(stepSize == 1){
	    return std::make_shared<Block<T> >(vec, 0, blockSize);
	}
	else {
	    return std::make_shared<SplitBlock<T> >(vec, 0, stepSize, blockSize);
	}
    }

    template <typename T>
    std::shared_ptr<MutableBlockBase<T> > makeBlock(std::vector<T>& vec, size_t stepSize, size_t blockSize)
    {
	if(stepSize == 0){
	    return std::make_shared<MBlockValue<T> >(vec[0], blockSize);
	}
	else if(stepSize == 1){
	    return std::make_shared<MBlock<T> >(vec, 0, blockSize);
	}
	else {
	    return std::make_shared<MSplitBlock<T> >(vec, 0, stepSize, blockSize);
	}
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

	auto blockIndex = seekBlockIndex(mIndex, second);
	block(blockIndex);
	second.block(blockIndex);
	for(*mIndex = 0; mIndex->pos() != mIndex->max(); mIndex->pp(blockIndex) )){
	    get() = mSecond.get();
	}
    }
    
    template <typename T, class... Ranges>
    BlockBase<T>& OperationMaster<T,Ranges...>::get()
    {
	block();
	return *mBlockPtr;
    }
    
    template <typename T, class... Ranges>
    const BlockBase<T>& OperationMaster<T,Ranges...>::get() const
    {
	block();
	return *mBlockPtr;
    }

    template <typename T, class... Ranges>
    std::vector<BTSS> OperationMaster<T,Ranges...>::block(const std::shared_ptr<IndexBase>& blockIndex) const
    {
	std::vector<BTSS> btv(1, getBlockType(mIndex, blockIndex, true) );
	mBlockPtr = makeBlock(mArrayRef.data(), btv[0].second, blockIndex->max());
	return btv;
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
    std::vector<BTSS> ConstOperationRoot<T,Ranges...>::block(const std::shared_ptr<IndexBase>& blockIndex) const
    {
	std::vector<BTSS> btv(1, getBlockType(mIndex, blockIndex, true) );
	mBlockPtr = makeBlock(mArrayRef.data(), btv[0].second, blockIndex->max());
	return btv;
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
	return *mBlockPtr;
    }

    template <typename T, class... Ranges>
    std::vector<BTSS> OperationRoot<T,Ranges...>::block(const std::shared_ptr<IndexBase>& blockIndex) const
    {
	std::vector<BTSS> btv(1, getBlockType(mIndex, blockIndex, true) );
	mBlockPtr = makeBlock(mArrayRef.data(), btv[0].second, blockIndex->max());
	return btv;
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
    std::vector<BTSS> Operation<T,Ranges...>::block(const std::shared_ptr<IndexBase>& blockIndex) const
    {
	std::vector<BTSS> btv;
	PackNum<sizeof...(Ranges)-1>::makeBlockTypeVec(btv, mOps, blockIndex);
	return btv;
    }

    template <typename T, class... Ranges>
    Operation<T,Ranges...>& Operation<T,Ranges...>::block() const
    {
	mBlockPtr->set( &mArrayRef[ (*mIndex)() ] );
	return *this;
    }

}
