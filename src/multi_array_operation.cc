// -*- C++ -*-

#include "multi_array_operation.h"

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }

    void seekIndexInst(std::shared_ptr<IndexBase> i, std::vector<std::shared_ptr<IndexBase> >& ivec)
    {
	for(size_t inum = 0; inum != i->rangePtr()->dim(); ++inum){
	    auto ii = i->getPtr(inum);
	    if(ii->type() == IndexType::MULTI or
	       ii->type() == IndexType::CONT){
		seekIndexInst(ii, ivec);
	    }
	    ivec.push_back(ii);
	}
    }

    BTSS getBlockType(std::shared_ptr<IndexBase> i,
		      std::shared_ptr<IndexBase> j,
		      bool first, size_t higherStepSize)
    {
	// returning BlockType and step size is redundant (change in the future)
	// stepSize == 0 => VALUE
	// stepSize == 1 => BLOCK
	// stepSize > 1 => SPLIT :)
	BTSS out(BlockType::VALUE, 0);
	size_t lastNum = i->rangePtr()->dim();
	for(size_t inum = 0; inum != lastNum; ++inum){
	    auto ii = i->getPtr(inum);
	    if(ii == j){
		
		if(inum == lastNum - 1 and first){
		    out = BTSS(BlockType::BLOCK, 1);
		}
		else {
		    first = false;
		    out = BTSS(BlockType::SPLIT, i->getStepSize(inum) * higherStepSize + out.second);
		}
		continue;
	    }
	    
	    if(ii->type() == IndexType::MULTI or
	       ii->type() == IndexType::CONT){

		BTSS tmp = getBlockType(ii, j, inum == lastNum - 1, i->getStepSize(inum) * higherStepSize);
		if(tmp.first != BlockType::VALUE){
		    out = tmp;
		}
	    }
	}
	return out;
    }

    template <typename T>
    std::shared_ptr<Block<T> > makeBlock(const std::vector<T>& vec, size_t stepSize, size_t blockSize)
    {
	return std::make_shared<Block<T> >(vec, 0, blockSize, stepSize);
    }

    template <typename T>
    std::shared_ptr<MBlock<T> > makeBlock(std::vector<T>& vec, size_t stepSize, size_t blockSize)
    {
	return std::make_shared<MBlock<T> >(vec, 0, blockSize, stepSize);
    }

    size_t getBTNum(const std::vector<BTSS>& mp, BlockType bt)
    {
	size_t out = 0;
	for(auto& xx: mp){
	    if(xx.first == bt){
		++out;
	    }
	}
	return out;
    }

    void minimizeAppearanceOfType(std::map<std::shared_ptr<IndexBase>, std::vector<BTSS> >& mp,
				  BlockType bt)
    {
	size_t minNum = getBTNum( mp.begin()->second, bt );
	for(auto& mm: mp){
	    size_t tmp = getBTNum( mm.second, bt );
	    if(tmp < minNum){
		minNum = tmp;
	    }
	}

	for(auto mit = mp.begin(); mit != mp.end(); ){
	    size_t tmp = getBTNum( mit->second, bt );
	    if(tmp > minNum){
		mit = mp.erase(mit);
	    }
	    else {
		++mit;
	    }
	}

    }
    
    template <class OpClass>
    std::shared_ptr<IndexBase> seekBlockIndex(std::shared_ptr<IndexBase> ownIdx,
					      const OpClass& second)
    {
	std::vector<std::shared_ptr<IndexBase> > ivec;
	seekIndexInst(ownIdx, ivec);
	std::map<std::shared_ptr<IndexBase>, std::vector<BTSS> > mp;
	
	for(auto& xx: ivec){
	    mp[xx] = second.block(xx);
	}
	// seek minimal number of VALUEs => guarantees absence of conflicting blocks
	minimizeAppearanceOfType(mp, BlockType::VALUE);
	// seek mininmal number of SPLITs => maximal vectorization possible
	minimizeAppearanceOfType(mp, BlockType::SPLIT);
	
	return mp.begin()->first;
    }

    
    /*********************************
     *   MultiArrayOperationBase     *
     *********************************/
    
    // purely virtual at the moment


    /***************************
     *   OperationTemplate     *
     ***************************/

    template <typename T, class OperationClass>
    OperationTemplate<T,OperationClass>::OperationTemplate(OperationClass* oc) : mOc(oc) {}
    
    template <typename T, class OperationClass>
    template <class Second>
    auto OperationTemplate<T,OperationClass>::operator+(const Second& in) const
	-> Operation<T,std::plus<T>,OperationClass,Second>
    {
    	return Operation<T,std::plus<T>,OperationClass,Second>(*mOc, in);
    }

    template <typename T, class OperationClass>
    template <class Second>
    auto OperationTemplate<T,OperationClass>::operator-(const Second& in) const
	-> Operation<T,std::minus<T>,OperationClass,Second>
    {
    	return Operation<T,std::minus<T>,OperationClass,Second>(*mOc, in);
    }
    
    template <typename T, class OperationClass>
    template <class Second>
    auto OperationTemplate<T,OperationClass>::operator*(const Second& in) const
	-> Operation<T,std::multiplies<T>,OperationClass,Second>
    {
    	return Operation<T,std::multiplies<T>,OperationClass,Second>(*mOc, in);
    }

    template <typename T, class OperationClass>
    template <class Second>
    auto OperationTemplate<T,OperationClass>::operator/(const Second& in) const
	-> Operation<T,std::divides<T>,OperationClass,Second>
    {
    	return Operation<T,std::divides<T>,OperationClass,Second>(*mOc, in);
    }

    /*************************
     *   OperationMaster     *
     *************************/

    template <typename T, class OpClass, class... Ranges>
    OperationMaster<T,OpClass,Ranges...>::
    OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OpClass& second,
		    std::shared_ptr<typename CRange::IndexType>& index) :
	mSecond(second), mArrayRef(ma), mIndex()
    {
	MultiRangeFactory<Ranges...> mrf( index->range() );
	std::shared_ptr<MultiRange<Ranges...> > mr =
	    std::dynamic_pointer_cast<MultiRange<Ranges...> >( mrf.create() );
	mIndex = std::make_shared<IndexType>( mr->begin() );
	(*mIndex) = *index;

	auto blockIndex = seekBlockIndex( mIndex, second);

	block(blockIndex);
	second.block(blockIndex);
	
	for(*mIndex = 0; mIndex->pos() != mIndex->max(); mIndex->pp(blockIndex) ){
	    get() = mSecond.get();
	}
    }
    
    template <typename T, class OpClass, class... Ranges>
    MBlock<T>& OperationMaster<T,OpClass,Ranges...>::get()
    {
	block();
	return *mBlockPtr;
    }
    
    template <typename T, class OpClass, class... Ranges>
    const Block<T>& OperationMaster<T,OpClass,Ranges...>::get() const
    {
	block();
	return *mBlockPtr;
    }

    template <typename T, class OpClass, class... Ranges>
    std::vector<BTSS> OperationMaster<T,OpClass,Ranges...>::block(const std::shared_ptr<IndexBase> blockIndex) const
    {
	std::vector<BTSS> btv(1, getBlockType(mIndex, blockIndex, true) );
	mBlockPtr = makeBlock(mArrayRef.datav(), btv[0].second, blockIndex->max());
	return btv;
    }

    template <typename T, class OpClass, class... Ranges>
    const OperationMaster<T,OpClass,Ranges...>& OperationMaster<T,OpClass,Ranges...>::block() const
    {
	mBlockPtr->set( mIndex->pos() );
	return *this;
    }
    
    /****************************
     *   ConstOperationRoot     *
     ****************************/

    template <typename T, class... Ranges>
    ConstOperationRoot<T,Ranges...>::
    ConstOperationRoot(const MultiArrayBase<T,Ranges...>& ma,
		       const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	OperationTemplate<T,ConstOperationRoot<T,Ranges...> >(this),
	mArrayRef(ma), mIndex( std::make_shared<IndexType>( mArrayRef.range() ) )
    {
	(*mIndex)(indices...);
    }

    template <typename T, class... Ranges>
    const Block<T>& ConstOperationRoot<T,Ranges...>::get() const
    {
	block();
	return *mBlockPtr;
    }

    template <typename T, class... Ranges>
    std::vector<BTSS> ConstOperationRoot<T,Ranges...>::block(const std::shared_ptr<IndexBase> blockIndex) const
    {
	std::vector<BTSS> btv(1, getBlockType(mIndex, blockIndex, true) );
	mBlockPtr = makeBlock(mArrayRef.datav(), btv[0].second, blockIndex->max());
	return btv;
    }

    template <typename T, class... Ranges>
    const ConstOperationRoot<T,Ranges...>& ConstOperationRoot<T,Ranges...>::block() const
    {
	mBlockPtr->set( (*mIndex)().pos() );
	return *this;
    }
    
    /***********************
     *   OperationRoot     *
     ***********************/
    
    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>::
    OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		  const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	OperationTemplate<T,OperationRoot<T,Ranges...> >(this),
	mArrayRef(ma), mIndex( std::make_shared<IndexType>( mArrayRef.range() ) )
    {
	(*mIndex)(indices...);
    }

    template <typename T, class... Ranges>
    template <class OpClass>
    OperationMaster<T,OpClass,Ranges...> OperationRoot<T,Ranges...>::operator=(const OpClass& in)
    {
	return OperationMaster<T,OpClass,Ranges...>(mArrayRef, in, mIndex);
    }

    template <typename T, class... Ranges>
    const MBlock<T>& OperationRoot<T,Ranges...>::get() const
    {
	block();
	return *mBlockPtr;
    }
    
    template <typename T, class... Ranges>
    MBlock<T>& OperationRoot<T,Ranges...>::get()
    {
	block();
	return *mBlockPtr;
    }

    template <typename T, class... Ranges>
    std::vector<BTSS> OperationRoot<T,Ranges...>::block(const std::shared_ptr<IndexBase> blockIndex) const
    {
	std::vector<BTSS> btv(1, getBlockType(mIndex, blockIndex, true) );
	mBlockPtr = makeBlock(mArrayRef.datav(), btv[0].second, blockIndex->max());
	return btv;
    }

    template <typename T, class... Ranges>
    const OperationRoot<T,Ranges...>& OperationRoot<T,Ranges...>::block() const
    {
	mBlockPtr->set( (*mIndex)().pos() );
	return *this;
    }
    
    /*******************
     *   Operation     *
     *******************/
    
    template <typename T, class OpFunction, class... Ops>
    Operation<T,OpFunction,Ops...>::Operation(const Ops&... ops) :
	OperationTemplate<T,Operation<T,OpFunction,Ops...> >(this),
	mOps(ops...) {}

    template <typename T, class OpFunction, class... Ops>
    const BlockResult<T>& Operation<T,OpFunction,Ops...>::get() const
    {
	mRes = std::move( PackNum<sizeof...(Ops)-1>::template unpackArgs<T,OpFunction>(mOps) );
	return mRes;
    }

    template <typename T, class OpFunction, class... Ops>
    std::vector<BTSS> Operation<T,OpFunction,Ops...>::block(const std::shared_ptr<IndexBase> blockIndex) const
    {
	std::vector<BTSS> btv;
	PackNum<sizeof...(Ops)-1>::makeBlockTypeVec(btv, mOps, blockIndex);
	return btv;
    }

    template <typename T, class OpFunction, class... Ops>
    const Operation<T,OpFunction,Ops...>& Operation<T,OpFunction,Ops...>::block() const
    {
	//mBlockPtr->set( mIndex->pos() );
	return *this;
    }

}
