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

    template <class OpClass>
    std::shared_ptr<VIWB> seekBlockIndex(std::shared_ptr<VIWB> ownIdx,
					 const OpClass& second);

    template <typename T, class OperationClass>
    class OperationTemplate
    {
    public:
	
	OperationTemplate(OperationClass* oc);
	
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
	OperationClass* mOc;
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
	
	MBlock<T>& get();
	const Block<T>& get() const;

	std::vector<BTSS> block(const std::shared_ptr<VIWB> blockIndex) const;
	const OperationMaster& block() const;
	
    protected:

	//void performAssignment(const OperationBase<T>& in);
	OpClass const& mSecond;
	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
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

	std::vector<BTSS> block(const std::shared_ptr<VIWB> blockIndex) const;
	const ConstOperationRoot& block() const;
	
    protected:
	
	MultiArrayBase<T,Ranges...> const& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
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

	std::vector<BTSS> block(const std::shared_ptr<VIWB> blockIndex) const;
	const OperationRoot& block() const;
	
    protected:
	
	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	std::shared_ptr<IndexType> mIndex;
	mutable MBlock<T> mBlock;
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

	std::vector<BTSS> block(const std::shared_ptr<VIWB> blockIndex) const;
	const Operation& block() const;
	
    protected:
	std::tuple<Ops...> mOps;
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

	std::vector<BTSS> block(const std::shared_ptr<VIWB> blockIndex) const;
	const Contraction& block() const;
	
    protected:

	Op mOp;
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

    template <class OpClass>
    std::shared_ptr<VIWB> seekBlockIndex(std::shared_ptr<VIWB> ownIdx,
					 const OpClass& second)
    {
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

    template <typename T, class OperationClass>
    template <class IndexType>
    auto OperationTemplate<T,OperationClass>::c(std::shared_ptr<IndexType>& ind) const
	-> Contraction<T,OperationClass,IndexType>
    {
	return Contraction<T,OperationClass,IndexType>(*mOc, ind);
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

	auto blockIndex = seekBlockIndex( make_viwb( mIndex ), second);
	std::intptr_t blockIndexNum = blockIndex->getPtrNum();
	
	block(blockIndex);
	second.block(blockIndex);
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
    std::vector<BTSS> OperationMaster<T,OpClass,Ranges...>::block(const std::shared_ptr<VIWB> blockIndex) const
    {
	std::vector<BTSS> btv(1, getBlockType( make_viwb( mIndex ), blockIndex, true) );
	mBlock = makeBlock(mArrayRef.data(), btv[0].second, blockIndex->max());
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
	OperationTemplate<T,ConstOperationRoot<T,Ranges...> >(this),
	mArrayRef(ma), mIndex( std::make_shared<IndexType>( mArrayRef.range() ) )
    {
	(*mIndex)(indices...);
    }

    template <typename T, class... Ranges>
    const Block<T>& ConstOperationRoot<T,Ranges...>::get() const
    {
	block();
	return mBlock;
    }

    template <typename T, class... Ranges>
    std::vector<BTSS> ConstOperationRoot<T,Ranges...>::block(const std::shared_ptr<VIWB> blockIndex) const
    {
	std::vector<BTSS> btv(1, getBlockType( make_viwb( mIndex ), blockIndex, true) );
	mBlock = makeBlock(mArrayRef.data(), btv[0].second, blockIndex->max());
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
	return mBlock;
    }
    
    template <typename T, class... Ranges>
    MBlock<T>& OperationRoot<T,Ranges...>::get()
    {
	block();
	return mBlock;
    }

    template <typename T, class... Ranges>
    std::vector<BTSS> OperationRoot<T,Ranges...>::block(const std::shared_ptr<VIWB> blockIndex) const
    {
	std::vector<BTSS> btv(1, getBlockType( make_viwb( mIndex ), blockIndex, true) );
	mBlock = makeBlock(mArrayRef.data(), btv[0].second, blockIndex->max());
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
	OperationTemplate<T,Operation<T,OpFunction,Ops...> >(this),
	mOps(ops...) {}

    template <typename T, class OpFunction, class... Ops>
    const BlockResult<T>& Operation<T,OpFunction,Ops...>::get() const
    {
	mRes = std::move( PackNum<sizeof...(Ops)-1>::template unpackArgs<T,OpFunction>(mOps) );
	return mRes;
    }

    template <typename T, class OpFunction, class... Ops>
    std::vector<BTSS> Operation<T,OpFunction,Ops...>::block(const std::shared_ptr<VIWB> blockIndex) const
    {
	std::vector<BTSS> btv;
	PackNum<sizeof...(Ops)-1>::makeBlockTypeVec(btv, mOps, blockIndex);
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
	OperationTemplate<T,Contraction<T,Op,IndexType> >(this),
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
    std::vector<BTSS> Contraction<T,Op,IndexType>::block(const std::shared_ptr<VIWB> blockIndex) const
    {
	return mOp.block(blockIndex);
    }

    template <typename T, class Op, class IndexType>
    const Contraction<T,Op,IndexType>& Contraction<T,Op,IndexType>::block() const
    {
	return *this;
    }
    
}

#endif
