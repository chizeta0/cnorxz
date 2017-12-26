// -*- C++ -*-

#ifndef __multi_array_operation_h__
#define __multi_array_operation_h__

#include <cstdlib>
#include <tuple>
#include <cmath>
#include <map>
#include <utility>

#include "base_def.h"
#include "mbase_def.h"

#include "block/block.h"
#include "operation_utils.h"
#include "ranges/rheader.h"
#include "pack_num.h"

#include "ranges/index_info.h"

namespace MultiArrayTools
{

    namespace
    {
	using namespace MultiArrayHelper;
    }
    
    /*
     * OperationBase
     * MutableOperationBase
     * 
     * OperationMaster : MutableOperationBase
     * 
     * OperationTemplate<...>
     * ConstOperationRoot : OperationBase, OperationTemplate<...>
     * OperationRoot : MutableOperationBase, 
     *                 OperationTemplate<...>
     * 
     */


    template <typename T>
    Block<T> makeBlock(const T* vec, size_t stepSize, size_t blockSize);
    
    template <typename T>
    MBlock<T> makeBlock(T* vec, size_t stepSize, size_t blockSize);

    // dont use this for now !!
    template <class OpClass>
    std::shared_ptr<VIWB> seekBlockIndex(std::shared_ptr<VIWB> ownIdx,
					 const OpClass& second);

    template <class OpClass>
    const IndexInfo* seekBlockIndex(const IndexInfo* ownII,
				    const OpClass& second);

    
    template <typename T, class OperationClass>
    class OperationTemplate
    {
    public:
	
	OperationClass& THIS() { return static_cast<OperationClass&>(*this); }
	const OperationClass& THIS() const { return static_cast<OperationClass const&>(*this); }
	
	template <class Second>
	auto operator+(const Second& in) const
	    -> Operation<T,std::plus<T>,OperationClass,Second>;
	
	template <class Second>
	auto operator-(const Second& in) const
	    -> Operation<T,std::minus<T>,OperationClass,Second>;
	
	template <class Second>
	auto operator*(const Second& in) const
	    -> Operation<T,std::multiplies<T>,OperationClass,Second>;
	
	template <class Second>
	auto operator/(const Second& in) const
	    -> Operation<T,std::divides<T>,OperationClass,Second>;

	template <class IndexType>
	auto c(std::shared_ptr<IndexType>& ind) const
	    -> Contraction<T,OperationClass,IndexType>;

    private:
	friend OperationClass;
	OperationTemplate() = default;
    };
    
    template <typename T, class OpClass, class... Ranges>
    class OperationMaster/* : public MutableOperationBase<T>*/
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename MultiRange<Ranges...>::IndexType IndexType;

	OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OpClass& second,
			std::shared_ptr<typename CRange::IndexType>& index);

	OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OpClass& second,
			std::shared_ptr<typename CRange::IndexType>& index,
			const IndexInfo* blockIndex);
	
	MBlock<T>& get();
	const Block<T>& get() const;

	std::vector<BTSS> block(const std::shared_ptr<VIWB> blockIndex, bool init = false) const;
	std::vector<BTSS> block(const IndexInfo* blockIndex, bool init = false) const;
	const OperationMaster& block() const;
	
    protected:

	std::shared_ptr<IndexType> mkIndex(std::shared_ptr<typename CRange::IndexType>& index);
	void performAssignment(std::intptr_t blockIndexNum);
	OpClass const& mSecond;
	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
	IndexInfo mIInfo;
	mutable MBlock<T> mBlock;
    };

    
    template <typename T, class... Ranges>
    class ConstOperationRoot : /*public OperationBase<T>,*/
			       public OperationTemplate<T,ConstOperationRoot<T,Ranges...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef OperationTemplate<T,ConstOperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename CRange::IndexType IndexType;
	
	ConstOperationRoot(const MultiArrayBase<T,Ranges...>& ma,
			   const std::shared_ptr<typename Ranges::IndexType>&... indices);
	
	const Block<T>& get() const;

	std::vector<BTSS> block(const std::shared_ptr<VIWB> blockIndex, bool init = false) const;
	std::vector<BTSS> block(const IndexInfo* blockIndex, bool init = false) const;
	const ConstOperationRoot& block() const;
	
    protected:

	std::shared_ptr<IndexType>
	mkIndex(const MultiArrayBase<T,Ranges...>& ma,
		const std::shared_ptr<typename Ranges::IndexType>&... indices);

	MultiArrayBase<T,Ranges...> const& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
	IndexInfo mIInfo;
 	mutable Block<T> mBlock;
    };
    
    template <typename T, class... Ranges>
    class OperationRoot : /*public MutableOperationBase<T>,*/
			  public OperationTemplate<T,OperationRoot<T,Ranges...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef OperationTemplate<T,OperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename CRange::IndexType IndexType;
	
	OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		      const std::shared_ptr<typename Ranges::IndexType>&... indices);

	template <class OpClass>
	OperationMaster<T,OpClass,Ranges...> operator=(const OpClass& in);
	
	const MBlock<T>& get() const;
	MBlock<T>& get();

	OperationRoot& set(const IndexInfo* blockIndex);
	OperationRoot& set(std::shared_ptr<VIWB> blockIndex);
	
	std::vector<BTSS> block(const std::shared_ptr<VIWB> blockIndex, bool init = false) const;
	std::vector<BTSS> block(const IndexInfo* blockIndex, bool init = false) const;
	const OperationRoot& block() const;
	
    protected:

	std::shared_ptr<IndexType>
	mkIndex(const MultiArrayBase<T,Ranges...>& ma,
		const std::shared_ptr<typename Ranges::IndexType>&... indices);

	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
	IndexInfo mIInfo;
	mutable MBlock<T> mBlock;
	std::shared_ptr<VIWB> mBlockIndex; // predefine to save time
	const IndexInfo* mBlockII; // predefine to save time
    };
    
    template <typename T, class OpFunction, class... Ops>
    class Operation : /*public OperationBase<T>,*/
		      public OperationTemplate<T,Operation<T,OpFunction,Ops...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef OperationTemplate<T,Operation<T,OpFunction,Ops...> > OT;
	typedef OpFunction F;
	
	Operation(const Ops&... ops);
	
	const BlockResult<T>& get() const;

	std::vector<BTSS> block(const std::shared_ptr<VIWB> blockIndex, bool init = false) const;
	std::vector<BTSS> block(const IndexInfo* blockIndex, bool init = false) const;
	const Operation& block() const;
	
    protected:
	std::tuple<Ops const&...> mOps;
	mutable BlockResult<T> mRes;
    };
    
    template <typename T, class Op, class IndexType>
    class Contraction : public OperationTemplate<T,Contraction<T,Op,IndexType> >
    {
    public:

	typedef T value_type;
	typedef OperationTemplate<T,Contraction<T,Op,IndexType> > OT;
	
	Contraction(const Op& op, std::shared_ptr<IndexType> ind);
	
	const BlockResult<T>& get() const;

	std::vector<BTSS> block(const std::shared_ptr<VIWB> blockIndex, bool init = false) const;
	std::vector<BTSS> block(const IndexInfo* blockIndex, bool init = false) const;
	const Contraction& block() const;
	
    protected:

	const Op& mOp;
	std::shared_ptr<IndexType> mInd;
	mutable BlockResult<T> mRes;
    };
    
}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }

    template <typename T>
    Block<T> makeBlock(const T* vec, size_t stepSize, size_t blockSize)
    {
	return Block<T>(vec, 0, blockSize, stepSize);
    }

    template <typename T>
    MBlock<T> makeBlock(T* vec, size_t stepSize, size_t blockSize)
    {
	return MBlock<T>(vec, 0, blockSize, stepSize);
    }

    // dont use this for now !!
    template <class OpClass>
    std::shared_ptr<VIWB> seekBlockIndex(std::shared_ptr<VIWB> ownIdx,
					 const OpClass& second)
    {
	assert(0); // dont use this for now !!
	std::vector<std::shared_ptr<VIWB> > ivec;
	seekIndexInst(ownIdx, ivec);
	std::map<std::shared_ptr<VIWB>, std::vector<BTSS> > mp;
	
	for(auto& xx: ivec){
	    mp[xx] = second.block(xx);
	}
	// seek minimal number of VALUEs => guarantees absence of conflicting blocks
	minimizeAppearanceOfType(mp, BlockType::VALUE);
	// seek mininmal number of SPLITs => maximal vectorization possible
	minimizeAppearanceOfType(mp, BlockType::SPLIT);
	
	return mp.begin()->first;
    }

    template <class OpClass>
    const IndexInfo* seekBlockIndex(const IndexInfo* ownII,
				    const OpClass& second)
	
    {
	const IndexInfo* ii = ownII;
	while(ii->type() == IndexType::CONT or
	      ii->type() == IndexType::MULTI){
	    ii = ii->getPtr(ii->dim()-1);
	}
	return ii;
    }

    
    /***************************
     *   OperationTemplate     *
     ***************************/
    
    template <typename T, class OperationClass>
    template <class Second>
    auto OperationTemplate<T,OperationClass>::operator+(const Second& in) const
	-> Operation<T,std::plus<T>,OperationClass,Second>
    {
    	return Operation<T,std::plus<T>,OperationClass,Second>(THIS(), in);
    }

    template <typename T, class OperationClass>
    template <class Second>
    auto OperationTemplate<T,OperationClass>::operator-(const Second& in) const
	-> Operation<T,std::minus<T>,OperationClass,Second>
    {
    	return Operation<T,std::minus<T>,OperationClass,Second>(THIS(), in);
    }
    
    template <typename T, class OperationClass>
    template <class Second>
    auto OperationTemplate<T,OperationClass>::operator*(const Second& in) const
	-> Operation<T,std::multiplies<T>,OperationClass,Second>
    {
    	return Operation<T,std::multiplies<T>,OperationClass,Second>(THIS(), in);
    }

    template <typename T, class OperationClass>
    template <class Second>
    auto OperationTemplate<T,OperationClass>::operator/(const Second& in) const
	-> Operation<T,std::divides<T>,OperationClass,Second>
    {
    	return Operation<T,std::divides<T>,OperationClass,Second>(THIS(), in);
    }

    template <typename T, class OperationClass>
    template <class IndexType>
    auto OperationTemplate<T,OperationClass>::c(std::shared_ptr<IndexType>& ind) const
	-> Contraction<T,OperationClass,IndexType>
    {
	return Contraction<T,OperationClass,IndexType>(THIS(), ind);
    }

    
    /*************************
     *   OperationMaster     *
     *************************/

    template <typename T, class OpClass, class... Ranges>
    OperationMaster<T,OpClass,Ranges...>::
    OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OpClass& second,
		    std::shared_ptr<typename CRange::IndexType>& index) :
	mSecond(second), mArrayRef(ma), mIndex(mkIndex(index)), mIInfo(*mIndex)
    {
	auto blockIndex = seekBlockIndex( &mIInfo, second);
	std::intptr_t blockIndexNum = blockIndex->getPtrNum();
	
	block(blockIndex, true);
	second.block(blockIndex, true);

	performAssignment(blockIndexNum);
    }

    template <typename T, class OpClass, class... Ranges>
    OperationMaster<T,OpClass,Ranges...>::
    OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OpClass& second,
		    std::shared_ptr<typename CRange::IndexType>& index,
		    const IndexInfo* blockIndex) :
	mSecond(second), mArrayRef(ma), mIndex(mkIndex(index)), mIInfo(*mIndex)
    {
	std::intptr_t blockIndexNum = blockIndex->getPtrNum();
	second.block(blockIndex, true);

	performAssignment(blockIndexNum);
    }

    template <typename T, class OpClass, class... Ranges>
    std::shared_ptr<typename OperationMaster<T,OpClass,Ranges...>::IndexType>
    OperationMaster<T,OpClass,Ranges...>::
    mkIndex(std::shared_ptr<typename CRange::IndexType>& index)
    {
	MultiRangeFactory<Ranges...> mrf( index->range() );
	std::shared_ptr<MultiRange<Ranges...> > mr =
	    std::dynamic_pointer_cast<MultiRange<Ranges...> >( mrf.create() );
	auto i = std::make_shared<IndexType>( mr->begin() );
	(*i) = *index;
	return i;
    }
    
    template <typename T, class OpClass, class... Ranges>
    void OperationMaster<T,OpClass,Ranges...>::performAssignment(std::intptr_t blockIndexNum)
    {
	//size_t cnt = 0;
	//std::clock_t cs = clock();
	for(*mIndex = 0; mIndex->pos() != mIndex->max(); mIndex->pp(blockIndexNum) ){
	    //std::clock_t c1 = clock();
	    block();
	    get() = mSecond.get();
	    //std::clock_t c2 = clock();
	    //cnt += c2 - c1;
	}
	//std::clock_t ce = clock();
	//std::cout << "total time = " << ce - cs << std::endl;
	//std::cout << "calc time  = " << cnt << std::endl;
    }
    
    template <typename T, class OpClass, class... Ranges>
    MBlock<T>& OperationMaster<T,OpClass,Ranges...>::get()
    {
	//block();
	return mBlock;
    }
    
    template <typename T, class OpClass, class... Ranges>
    const Block<T>& OperationMaster<T,OpClass,Ranges...>::get() const
    {
	//block();
	return mBlock;
    }

    template <typename T, class OpClass, class... Ranges>
    std::vector<BTSS> OperationMaster<T,OpClass,Ranges...>::block(const std::shared_ptr<VIWB> blockIndex, bool init) const
    {
	std::vector<BTSS> btv(1, getBlockType( make_viwb( mIndex ), blockIndex, true) );
	if(init){
	    mBlock = makeBlock(mArrayRef.data(), btv[0].second, blockIndex->max());
	}
	return btv;
    }

    template <typename T, class OpClass, class... Ranges>
    std::vector<BTSS> OperationMaster<T,OpClass,Ranges...>::block(const IndexInfo* blockIndex, bool init) const
    {
	std::vector<BTSS> btv(1, getBlockType( &mIInfo, blockIndex, true) );
	if(init){
	    mBlock = makeBlock(mArrayRef.data(), btv[0].second, blockIndex->max());
	}
	return btv;
    }

    
    template <typename T, class OpClass, class... Ranges>
    const OperationMaster<T,OpClass,Ranges...>& OperationMaster<T,OpClass,Ranges...>::block() const
    {
	mBlock.set( mIndex->pos() );
	return *this;
    }
    
    /****************************
     *   ConstOperationRoot     *
     ****************************/

    template <typename T, class... Ranges>
    ConstOperationRoot<T,Ranges...>::
    ConstOperationRoot(const MultiArrayBase<T,Ranges...>& ma,
		       const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	//OperationTemplate<T,ConstOperationRoot<T,Ranges...> >(this),
	mArrayRef(ma), mIndex( mkIndex(ma,indices...) ), mIInfo(*mIndex)
    {}

    template <typename T, class... Ranges>
    std::shared_ptr<typename ConstOperationRoot<T,Ranges...>::IndexType>
    ConstOperationRoot<T,Ranges...>::
    mkIndex(const MultiArrayBase<T,Ranges...>& ma,
	    const std::shared_ptr<typename Ranges::IndexType>&... indices)
    {
	auto i = std::make_shared<IndexType>( ma.range() );
	(*mIndex)(indices...);
	return i;
    }
    
    template <typename T, class... Ranges>
    const Block<T>& ConstOperationRoot<T,Ranges...>::get() const
    {
	block();
	return mBlock;
    }

    template <typename T, class... Ranges>
    std::vector<BTSS> ConstOperationRoot<T,Ranges...>::block(const std::shared_ptr<VIWB> blockIndex, bool init) const
    {
	assert(0);
	std::vector<BTSS> btv(1, getBlockType( make_viwb( mIndex ), blockIndex, true) );
	if(init){
	    mBlock = makeBlock(mArrayRef.data(), btv[0].second, blockIndex->max());
	}
	return btv;
    }

    template <typename T, class... Ranges>
    std::vector<BTSS> ConstOperationRoot<T,Ranges...>::block(const IndexInfo* blockIndex, bool init) const
    {
	std::vector<BTSS> btv(1, getBlockType( &mIInfo, blockIndex, true) );
	if(init){
	    mBlock = makeBlock(mArrayRef.data(), btv[0].second, blockIndex->max());
	}
	return btv;
    }
    
    template <typename T, class... Ranges>
    const ConstOperationRoot<T,Ranges...>& ConstOperationRoot<T,Ranges...>::block() const
    {
	mBlock.set( (*mIndex)().pos() );
	return *this;
    }
    
    /***********************
     *   OperationRoot     *
     ***********************/
    
    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>::
    OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		  const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	//OperationTemplate<T,OperationRoot<T,Ranges...> >(this),
	mArrayRef(ma), mIndex( mkIndex( ma, indices... ) ), mIInfo(*mIndex),
	mBlockIndex(nullptr), mBlockII(nullptr)
    {}

    template <typename T, class... Ranges>
    std::shared_ptr<typename OperationRoot<T,Ranges...>::IndexType>
    OperationRoot<T,Ranges...>::
    mkIndex(const MultiArrayBase<T,Ranges...>& ma,
	    const std::shared_ptr<typename Ranges::IndexType>&... indices)
    {
	auto i = std::make_shared<IndexType>( ma.range() );
	(*mIndex)(indices...);
	return i;
    }
    
    template <typename T, class... Ranges>
    template <class OpClass>
    OperationMaster<T,OpClass,Ranges...> OperationRoot<T,Ranges...>::operator=(const OpClass& in)
    {
	if(mBlockII != nullptr){
	    return OperationMaster<T,OpClass,Ranges...>(mArrayRef, in, mIndex, mBlockII);
	}
	else {
	    return OperationMaster<T,OpClass,Ranges...>(mArrayRef, in, mIndex);
	}
    }

    template <typename T, class... Ranges>
    const MBlock<T>& OperationRoot<T,Ranges...>::get() const
    {
	block();
	return mBlock;
    }
    
    template <typename T, class... Ranges>
    MBlock<T>& OperationRoot<T,Ranges...>::get()
    {
	block();
	return mBlock;
    }

    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>&
    OperationRoot<T,Ranges...>::set(std::shared_ptr<VIWB> blockIndex)
    {
	mBlockIndex = blockIndex;
	return *this;
    }

    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>&
    OperationRoot<T,Ranges...>::set(const IndexInfo* blockIndex)
    {
	mBlockII = blockIndex;
	return *this;
    }

    
    template <typename T, class... Ranges>
    std::vector<BTSS> OperationRoot<T,Ranges...>::block(const std::shared_ptr<VIWB> blockIndex, bool init) const
    {
	assert(0);
	std::vector<BTSS> btv(1, getBlockType( make_viwb( mIndex ), blockIndex, true) );
	if(init){
	    mBlock = makeBlock(mArrayRef.data(), btv[0].second, blockIndex->max());
	}
	return btv;
    }

    template <typename T, class... Ranges>
    std::vector<BTSS> OperationRoot<T,Ranges...>::block(const IndexInfo* blockIndex, bool init) const
    {
	std::vector<BTSS> btv(1, getBlockType( &mIInfo, blockIndex, true) );
	if(init){
	    mBlock = makeBlock(mArrayRef.data(), btv[0].second, blockIndex->max());
	}
	return btv;
    }
    
    template <typename T, class... Ranges>
    const OperationRoot<T,Ranges...>& OperationRoot<T,Ranges...>::block() const
    {
	mBlock.set( (*mIndex)().pos() );
	return *this;
    }
    
    /*******************
     *   Operation     *
     *******************/
    
    template <typename T, class OpFunction, class... Ops>
    Operation<T,OpFunction,Ops...>::Operation(const Ops&... ops) :
	//OperationTemplate<T,Operation<T,OpFunction,Ops...> >(this),
	mOps(ops...) {}

    template <typename T, class OpFunction, class... Ops>
    const BlockResult<T>& Operation<T,OpFunction,Ops...>::get() const
    {
	PackNum<sizeof...(Ops)-1>::template unpackArgs<T,OpFunction>(mRes, mOps);
	return mRes;
    }

    template <typename T, class OpFunction, class... Ops>
    std::vector<BTSS> Operation<T,OpFunction,Ops...>::block(const std::shared_ptr<VIWB> blockIndex, bool init) const
    {
	assert(0);
	std::vector<BTSS> btv;
	PackNum<sizeof...(Ops)-1>::makeBlockTypeVec(btv, mOps, blockIndex, init);
	if(init){
	    mRes.init(blockIndex->max());
	}
	return btv;
    }

    template <typename T, class OpFunction, class... Ops>
    std::vector<BTSS> Operation<T,OpFunction,Ops...>::block(const IndexInfo* blockIndex, bool init) const
    {
	std::vector<BTSS> btv;
	PackNum<sizeof...(Ops)-1>::makeBlockTypeVec(btv, mOps, blockIndex, init);
	if(init){
	    mRes.init(blockIndex->max());
	}
	return btv;
    }

    
    template <typename T, class OpFunction, class... Ops>
    const Operation<T,OpFunction,Ops...>& Operation<T,OpFunction,Ops...>::block() const
    {
	//mBlock.set( mIndex->pos() );
	return *this;
    }

    /*********************
     *   Contraction     *
     *********************/

    template <typename T, class Op, class IndexType>
    Contraction<T,Op,IndexType>::Contraction(const Op& op, std::shared_ptr<IndexType> ind) :
	//OperationTemplate<T,Contraction<T,Op,IndexType> >(this),
	mOp(op),
	mInd(ind) {}

    template <typename T, class Op, class IndexType>
    const BlockResult<T>& Contraction<T,Op,IndexType>::get() const
    {
	BlockBinaryOpSelf<T,std::plus<T>,decltype(mOp.get())> f(mRes);
	for(*mInd = 0; mInd->pos() != mInd->max(); ++(*mInd)){
	    f(mOp.get());
	}
	return mRes;
    }
    
    template <typename T, class Op, class IndexType>
    std::vector<BTSS> Contraction<T,Op,IndexType>::block(const std::shared_ptr<VIWB> blockIndex, bool init) const
    {
	if(init){
	    mRes.init(blockIndex->max());
	}
	return mOp.block(blockIndex, init);
    }

    template <typename T, class Op, class IndexType>
    std::vector<BTSS> Contraction<T,Op,IndexType>::block(const IndexInfo* blockIndex, bool init) const
    {
	if(init){
	    mRes.init(blockIndex->max());
	}
	return mOp.block(blockIndex, init);
    }
    
    template <typename T, class Op, class IndexType>
    const Contraction<T,Op,IndexType>& Contraction<T,Op,IndexType>::block() const
    {
	return *this;
    }
    
}

#endif
