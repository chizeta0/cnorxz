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
#include "arith.h"

namespace MultiArrayTools
{

    namespace
    {
	using namespace MultiArrayHelper;
    }
    
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
	    -> Operation<T,plus<T>,OperationClass,Second>;
	
	template <class Second>
	auto operator-(const Second& in) const
	    -> Operation<T,minus<T>,OperationClass,Second>;
	
	template <class Second>
	auto operator*(const Second& in) const
	    -> Operation<T,multiplies<T>,OperationClass,Second>;
	
	template <class Second>
	auto operator/(const Second& in) const
	    -> Operation<T,divides<T>,OperationClass,Second>;

	template <class IndexType>
	auto c(std::shared_ptr<IndexType>& ind) const
	    -> Contraction<T,OperationClass,IndexType>;

    private:
	friend OperationClass;
	OperationTemplate() = default;
    };
    
    template <typename T, class OpClass, class... Ranges>
    class OperationMaster
    {
    public:

	class AssignmentExpr
	{
	private:
	    AssignmentExpr() = default;
	    //AssignmentExpr(const AssignmentExpr& in) = default;
	    //AssignmentExpr& operator=(const AssignmentExpr& in) = default;
	    	    
	    OperationMaster& mM;
	    const OpClass& mSec;

	public:
	    static size_t layer() { return 0; }

	    static const size_t LAYER = 0;
	    static const size_t SIZE = OpClass::SIZE;
	    typedef decltype(mSec.rootSteps()) ETuple;
	    
	    AssignmentExpr(OperationMaster& m, const OpClass& sec);

	    AssignmentExpr(AssignmentExpr&& in) = default;
	    //AssignmentExpr& operator=(const AssignmentExpr&& in) = default;
	    
	    //inline void operator()(size_t start = 0);
	    inline void operator()(size_t start, const ETuple& last);

	    ETuple rootSteps(std::intptr_t iPtrNum = 0);
	    
	};
	
	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename MultiRange<Ranges...>::IndexType IndexType;
	typedef MBlock<T> bType;	

	static size_t rootNum() { return 1; }
	
	OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OpClass& second,
			std::shared_ptr<typename CRange::IndexType>& index);

	OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OpClass& second,
			std::shared_ptr<typename CRange::IndexType>& index,
			const IndexInfo* blockIndex);
	
	MBlock<T>& get();
	const Block<T>& get() const;

	inline T& get(size_t pos);
	inline const T& get(size_t pos) const;
	
	std::vector<BTSS> block(const IndexInfo* blockIndex, bool init = false) const;
	const OperationMaster& block() const;

    private:

	std::shared_ptr<IndexType> mkIndex(std::shared_ptr<typename CRange::IndexType>& index);
	void performAssignment(std::intptr_t blockIndexNum);
	OpClass const& mSecond;
	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	std::vector<T>& mData;
	std::shared_ptr<IndexType> mIndex;
	IndexInfo mIInfo;
	mutable bType mBlock;
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
	typedef Block<T> bType;

	static size_t rootNum() { return 1; }
	static const size_t SIZE = 1;
	
	ConstOperationRoot(const MultiArrayBase<T,Ranges...>& ma,
			   const std::shared_ptr<typename Ranges::IndexType>&... indices);
	
	const Block<T>& get() const;
	
	template <class ET, size_t SITE>
	inline const T& get(const ET& pos) const;

	std::vector<BTSS> block(const IndexInfo* blockIndex, bool init = false) const;
	const ConstOperationRoot& block() const;

	std::tuple<size_t> rootSteps(std::intptr_t iPtrNum = 0) const; // nullptr for simple usage with decltype

	template <class Expr>
	Expr&& loop(Expr&& exp) const;
		
    private:

	std::shared_ptr<IndexType>
	mkIndex(const MultiArrayBase<T,Ranges...>& ma,
		const std::shared_ptr<typename Ranges::IndexType>&... indices);

	MultiArrayBase<T,Ranges...> const& mArrayRef;
	const std::vector<T>& mData;
	std::shared_ptr<IndexType> mIndex;
	IndexInfo mIInfo;
 	mutable bType mBlock;
    };
    
    template <typename T, class... Ranges>
    class OperationRoot : public OperationTemplate<T,OperationRoot<T,Ranges...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef OperationTemplate<T,OperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<Ranges...> CRange;
	typedef typename CRange::IndexType IndexType;
	typedef MBlock<T> bType;

	static size_t rootNum() { return 1; }
	static const size_t SIZE = 1;
	
	OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		      const std::shared_ptr<typename Ranges::IndexType>&... indices);

	template <class OpClass>
	OperationMaster<T,OpClass,Ranges...> operator=(const OpClass& in);
	
	const MBlock<T>& get() const;
	MBlock<T>& get();

	template <class ET, size_t SITE>
	inline const T& get(const ET& pos) const;

	template <class ET, size_t SITE>
	inline T& get(const ET& pos);
	
	OperationRoot& set(const IndexInfo* blockIndex);
	std::vector<BTSS> block(const IndexInfo* blockIndex, bool init = false) const;
	const OperationRoot& block() const;

	std::tuple<size_t> rootSteps(std::intptr_t iPtrNum = 0) const; // nullptr for simple usage with decltype

	template <class Expr>
	Expr&& loop(Expr&& exp) const;
	
    private:

	std::shared_ptr<IndexType>
	mkIndex(const MultiArrayBase<T,Ranges...>& ma,
		const std::shared_ptr<typename Ranges::IndexType>&... indices);

	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	std::vector<T>& mData;
	std::shared_ptr<IndexType> mIndex;
	IndexInfo mIInfo;
	mutable bType mBlock;
	const IndexInfo* mBlockII; // predefine to save time
    };

    template <class Op>
    size_t sumRootNum()
    {
	return typename Op::rootNum();
    }
    
    template <class Op1, class Op2, class... Ops>
    size_t sumRootNum()
    {
	return typename Op1::rootNum() + sumRootNum<Op2,Ops...>();
    }

    template <size_t N>
    struct RootSumN
    {
	template <class Op1, class... Ops>
	struct rs
	{
	    static const size_t SIZE = Op1::SIZE + RootSumN<N-1>::template rs<Ops...>::SIZE;
	};
    };

    template <>
    struct RootSumN<0>
    {
	template <class Op1>
	struct rs
	{
	    static const size_t SIZE = Op1::SIZE;
	};
    };

    
    template <class... Ops>
    struct RootSum
    {
	static const size_t SIZE = RootSumN<sizeof...(Ops)-1>::template rs<Ops...>::SIZE;
    };

    
    template <typename T, class OpFunction, class... Ops>
    class Operation : public OperationTemplate<T,Operation<T,OpFunction,Ops...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef OperationTemplate<T,Operation<T,OpFunction,Ops...> > OT;
	typedef OpFunction F;
	typedef BlockResult<T> bType;

	static size_t rootNum() { return sumRootNum<Ops...>(); }
	static const size_t SIZE = RootSum<Ops...>::SIZE;

    private:
	std::tuple<Ops const&...> mOps;
	mutable bType mRes;

    public:
	typedef decltype(PackNum<sizeof...(Ops)-1>::mkStepTuple(0, mOps)) ETuple;
	//typedef decltype(PackNum<sizeof...(Ops)-1>::template mkLoopType<Ops...>) LType;
	
	Operation(const Ops&... ops);
	
	const BlockResult<T>& get() const;
	
	template <class ET, size_t SITE>
	inline T get(const ET& pos) const;

	std::vector<BTSS> block(const IndexInfo* blockIndex, bool init = false) const;
	const Operation& block() const;

	auto rootSteps(std::intptr_t iPtrNum = 0) const // nullptr for simple usage with decltype
	    -> decltype(PackNum<sizeof...(Ops)-1>::mkStepTuple(iPtrNum, mOps));

	template <class Expr>
	auto loop(Expr&& exp) const
	    -> decltype(PackNum<sizeof...(Ops)-1>::mkLoop( mOps, std::forward<Expr>( exp ) ))&&;
	
    };
    
    template <typename T, class Op, class IndexType>
    class Contraction : public OperationTemplate<T,Contraction<T,Op,IndexType> >
    {
    public:

	typedef T value_type;
	typedef OperationTemplate<T,Contraction<T,Op,IndexType> > OT;
	typedef BlockResult<T> bType;

	static size_t rootNum() { return typename Op::rootNum(); }
	static const size_t SIZE = Op::SIZE;
	
    private:

	const Op& mOp;
	std::shared_ptr<IndexType> mInd;
	mutable bType mRes;

    public:
	typedef decltype(mOp.rootSteps(0)) ETuple;
	
	Contraction(const Op& op, std::shared_ptr<IndexType> ind);
	
	const BlockResult<T>& get() const;

	template <class ET, size_t SITE>
	inline T get(const ET& pos) const;

	std::vector<BTSS> block(const IndexInfo* blockIndex, bool init = false) const;
	const Contraction& block() const;

	auto rootSteps(std::intptr_t iPtrNum = 0) const  // nullptr for simple usage with decltype
	    -> decltype(mOp.rootSteps(iPtrNum));

	template <class Expr>
	auto loop(Expr&& exp) const -> decltype(mInd->iforh(exp))&&;
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
	-> Operation<T,plus<T>,OperationClass,Second>
    {
    	return Operation<T,plus<T>,OperationClass,Second>(THIS(), in);
    }

    template <typename T, class OperationClass>
    template <class Second>
    auto OperationTemplate<T,OperationClass>::operator-(const Second& in) const
	-> Operation<T,minus<T>,OperationClass,Second>
    {
    	return Operation<T,minus<T>,OperationClass,Second>(THIS(), in);
    }
    
    template <typename T, class OperationClass>
    template <class Second>
    auto OperationTemplate<T,OperationClass>::operator*(const Second& in) const
	-> Operation<T,multiplies<T>,OperationClass,Second>
    {
    	return Operation<T,multiplies<T>,OperationClass,Second>(THIS(), in);
    }

    template <typename T, class OperationClass>
    template <class Second>
    auto OperationTemplate<T,OperationClass>::operator/(const Second& in) const
	-> Operation<T,divides<T>,OperationClass,Second>
    {
    	return Operation<T,divides<T>,OperationClass,Second>(THIS(), in);
    }

    template <typename T, class OperationClass>
    template <class IndexType>
    auto OperationTemplate<T,OperationClass>::c(std::shared_ptr<IndexType>& ind) const
	-> Contraction<T,OperationClass,IndexType>
    {
	return Contraction<T,OperationClass,IndexType>(THIS(), ind);
    }

    /*****************************************
     *   OperationMaster::AssignmentExpr     *
     *****************************************/

    template <typename T, class OpClass, class... Ranges>
    OperationMaster<T,OpClass,Ranges...>::AssignmentExpr::
    AssignmentExpr(OperationMaster& m, const OpClass& sec) :
	mM(m), mSec(sec) {}

    /*
    template <typename T, class OpClass, class... Ranges>
    inline void OperationMaster<T,OpClass,Ranges...>::AssignmentExpr::
    operator()(size_t start)
    {
	
    }*/

    template <typename T, class OpClass, class... Ranges>
    inline void OperationMaster<T,OpClass,Ranges...>::AssignmentExpr::
    operator()(size_t start, const ETuple& last)
    {
	mM.get(start) = mSec.template get<ETuple,OpClass::SIZE-1>(last);
    }
    
    template <typename T, class OpClass, class... Ranges>
    typename OperationMaster<T,OpClass,Ranges...>::AssignmentExpr::ETuple
    OperationMaster<T,OpClass,Ranges...>::AssignmentExpr::
    rootSteps(std::intptr_t iPtrNum)
    {
	return mSec.rootSteps(iPtrNum);
    }
    
    
    /*************************
     *   OperationMaster     *
     *************************/

    template <typename T, class OpClass, class... Ranges>
    OperationMaster<T,OpClass,Ranges...>::
    OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OpClass& second,
		    std::shared_ptr<typename CRange::IndexType>& index) :
	mSecond(second), mArrayRef(ma), mData(mArrayRef.datav()),
	mIndex(mkIndex(index)), mIInfo(*mIndex)
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
	mSecond(second), mArrayRef(ma), mData(mArrayRef.datav()),
	mIndex(mkIndex(index)), mIInfo(*mIndex)
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
#define XX_USE_NEW_LOOP_ROUTINE_XX
#ifdef XX_USE_NEW_LOOP_ROUTINE_XX
	// === N E W ===
	AssignmentExpr ae(*this, mSecond); // Expression to be executed within loop
	//const auto hiddenLoop = mSecond.loop(AssignmentExpr(*this, mSecond)); // hidden loop within 'mSecond' e.g. contractions
	const auto loop = mIndex->ifor
	    ( mSecond.template loop<AssignmentExpr>
	      ( std::move(ae) ) ); // init overall loop(s) 
	loop(); // execute overall loop(s) and so internal hidden loops and so the inherited expressions
#else
	// === O L D ===
	for(*mIndex = 0; mIndex->pos() != mIndex->max(); mIndex->pp(blockIndexNum) ){
	    block();
	    get() = mSecond.get();
	}
#endif
    }
    
    template <typename T, class OpClass, class... Ranges>
    MBlock<T>& OperationMaster<T,OpClass,Ranges...>::get()
    {
	return mBlock;
    }
    
    template <typename T, class OpClass, class... Ranges>
    const Block<T>& OperationMaster<T,OpClass,Ranges...>::get() const
    {
	return mBlock;
    }

    template <typename T, class OpClass, class... Ranges>
    inline T& OperationMaster<T,OpClass,Ranges...>::get(size_t pos)
    {
	//assert(pos < mIndex->max());
	//if(pos >= mIndex->max()) { VCHECK(pos); VCHECK(mIndex->max()); assert(0); }
	//VCHECK(pos);
	return mData[pos];
    }

    template <typename T, class OpClass, class... Ranges>
    inline const T& OperationMaster<T,OpClass,Ranges...>::get(size_t pos) const
    {
	//VCHECK(pos);
	return mData[pos];
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
	mArrayRef(ma), mData(mArrayRef.datav()),
	mIndex( mkIndex(ma,indices...) ), mIInfo(*mIndex)
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
    template <class ET, size_t SITE>
    inline const T& ConstOperationRoot<T,Ranges...>::get(const ET& pos) const
    {
	return mData[std::get<SITE>(pos)];
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

    template <typename T, class... Ranges>
    std::tuple<size_t> ConstOperationRoot<T,Ranges...>::rootSteps(std::intptr_t iPtrNum) const
    {
	return std::tuple<size_t>(0ul); // !!!!!!
    }


    template <typename T, class... Ranges>
    template <class Expr>
    Expr&& ConstOperationRoot<T,Ranges...>::loop(Expr&& exp) const
    {
	return std::forward<Expr>(exp);
    }
    
    /***********************
     *   OperationRoot     *
     ***********************/
    
    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>::
    OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		  const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	//OperationTemplate<T,OperationRoot<T,Ranges...> >(this),
	mArrayRef(ma), mData(mArrayRef.datav()),
	mIndex( mkIndex( ma, indices... ) ), mIInfo(*mIndex),
	mBlockII(nullptr)
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
    template <class ET, size_t SITE>
    inline const T& OperationRoot<T,Ranges...>::get(const ET& pos) const
    {
	return mData[std::get<SITE>( pos )];
    }

    template <typename T, class... Ranges>
    template <class ET, size_t SITE>
    inline T& OperationRoot<T,Ranges...>::get(const ET& pos)
    {
	return mData[std::get<SITE>( pos )];
    }
    
    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>&
    OperationRoot<T,Ranges...>::set(const IndexInfo* blockIndex)
    {
	mBlockII = blockIndex;
	return *this;
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

    template <typename T, class... Ranges>
    std::tuple<size_t> OperationRoot<T,Ranges...>::rootSteps(std::intptr_t iPtrNum) const
    {
	return std::tuple<size_t>(0ul); // !!!!!!
    }

    template <typename T, class... Ranges>
    template <class Expr>
    Expr&& OperationRoot<T,Ranges...>::loop(Expr&& exp) const
    {
	return std::forward<Expr>(exp);
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
    template <class ET, size_t SITE>
    inline T Operation<T,OpFunction,Ops...>::get(const ET& pos) const
    {
	typedef std::tuple<Ops const&...> OpTuple;
	return PackNum<sizeof...(Ops)-2>::
	    template mkOpExpr<T,ET,OpTuple,OpFunction,SITE>(pos, mOps);
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

    template <typename T, class OpFunction, class... Ops>
    auto Operation<T,OpFunction,Ops...>::rootSteps(std::intptr_t iPtrNum) const
	-> decltype(PackNum<sizeof...(Ops)-1>::mkStepTuple(iPtrNum, mOps))
    {
	return PackNum<sizeof...(Ops)-1>::mkStepTuple(iPtrNum, mOps);
    }

    template <typename T, class OpFunction, class... Ops>
    template <class Expr>
    auto Operation<T,OpFunction,Ops...>::loop(Expr&& exp) const
	-> decltype(PackNum<sizeof...(Ops)-1>::mkLoop( mOps, std::forward<Expr>( exp ) ))&&
    {
	typedef decltype(PackNum<sizeof...(Ops)-1>::mkLoop( mOps, std::forward<Expr>( exp ) ) )
	    LType;
	return std::forward<LType>
	    ( PackNum<sizeof...(Ops)-1>::mkLoop( mOps, std::forward<Expr>( exp ) ) );
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

    // forward loop !!!!
    template <typename T, class Op, class IndexType>
    template <class ET, size_t SITE>
    inline T Contraction<T,Op,IndexType>::get(const ET& pos) const
    {
	return mOp.template get<ET,SITE>(pos);
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

    template <typename T, class Op, class IndexType>
    auto Contraction<T,Op,IndexType>::rootSteps(std::intptr_t iPtrNum) const
	-> decltype(mOp.rootSteps(iPtrNum))
    {
	return mOp.rootSteps(iPtrNum);
    }

    template <typename T, class Op, class IndexType>
    template <class Expr>
    auto Contraction<T,Op,IndexType>::loop(Expr&& exp) const -> decltype(mInd->iforh(exp))&&
    {
	typedef decltype(mInd->iforh(exp)) LType;
	LType&& loop = mInd->iforh(exp);
	return std::forward<LType>( loop );
    }

}

#endif
