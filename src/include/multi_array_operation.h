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

//#include "block/block.h"
//#include "operation_utils.h"
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
	    	    
	    OperationMaster& mM;
	    const OpClass& mSec;

	public:

	    static constexpr size_t LAYER = 0;
	    static constexpr size_t SIZE = OpClass::SIZE;
	    typedef decltype(mSec.rootSteps()) ExtType;
	    
	    AssignmentExpr(OperationMaster& m, const OpClass& sec);

	    AssignmentExpr(const AssignmentExpr& in) = default;
	    AssignmentExpr(AssignmentExpr&& in) = default;
	    
	    inline void operator()(size_t start = 0) const; 
	    inline void operator()(size_t start, ExtType last) const;

	    auto rootSteps(std::intptr_t iPtrNum = 0) const -> ExtType;
	    
	};
	
	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef ContainerRange<T,Ranges...> CRange;
	typedef typename MultiRange<Ranges...>::IndexType IndexType;
	
	OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OpClass& second,
			std::shared_ptr<typename CRange::IndexType>& index);

	OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OpClass& second,
			std::shared_ptr<typename CRange::IndexType>& index,
			const IndexInfo* blockIndex);

	inline void set(size_t pos, T val) { mDataPtr[pos] = val; }
	inline void add(size_t pos, T val) { mDataPtr[pos] += val; }
	inline T get(size_t pos) const;

    private:

	std::shared_ptr<IndexType> mkIndex(std::shared_ptr<typename CRange::IndexType>& index);
	void performAssignment(std::intptr_t blockIndexNum);
	OpClass const& mSecond;
	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	T* mDataPtr;
	std::shared_ptr<IndexType> mIndex;
	IndexInfo mIInfo;
    };

    
    template <typename T, class... Ranges>
    class ConstOperationRoot : /*public OperationBase<T>,*/
			       public OperationTemplate<T,ConstOperationRoot<T,Ranges...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef OperationTemplate<T,ConstOperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<T,Ranges...> CRange;
	typedef typename CRange::IndexType IndexType;

	static constexpr size_t SIZE = 1;
	
	ConstOperationRoot(const MultiArrayBase<T,Ranges...>& ma,
			   const std::shared_ptr<typename Ranges::IndexType>&... indices);
	
	template <class ET>
	inline T get(ET pos) const;

	MExt<void> rootSteps(std::intptr_t iPtrNum = 0) const; // nullptr for simple usage with decltype

	template <class Expr>
	Expr loop(Expr exp) const;
		
    private:

	std::shared_ptr<IndexType>
	mkIndex(const MultiArrayBase<T,Ranges...>& ma,
		const std::shared_ptr<typename Ranges::IndexType>&... indices);

	MultiArrayBase<T,Ranges...> const& mArrayRef;
	const T* mDataPtr;
	std::shared_ptr<IndexType> mIndex;
	IndexInfo mIInfo;
    };
    
    template <typename T, class... Ranges>
    class OperationRoot : public OperationTemplate<T,OperationRoot<T,Ranges...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef OperationTemplate<T,OperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<T,Ranges...> CRange;
	typedef typename CRange::IndexType IndexType;

	static constexpr size_t SIZE = 1;
	
	OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		      const std::shared_ptr<typename Ranges::IndexType>&... indices);

	template <class OpClass>
	OperationMaster<T,OpClass,Ranges...> operator=(const OpClass& in);

	template <class ET>
	inline T get(ET pos) const;

	MExt<void> rootSteps(std::intptr_t iPtrNum = 0) const; // nullptr for simple usage with decltype

	template <class Expr>
	Expr loop(Expr exp) const;
	
    private:

	std::shared_ptr<IndexType>
	mkIndex(const MultiArrayBase<T,Ranges...>& ma,
		const std::shared_ptr<typename Ranges::IndexType>&... indices);

	MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	T* mDataPtr;
	std::shared_ptr<IndexType> mIndex;
	IndexInfo mIInfo;
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
	    static constexpr size_t SIZE = Op1::SIZE + RootSumN<N-1>::template rs<Ops...>::SIZE;
	};
    };

    template <>
    struct RootSumN<0>
    {
	template <class Op1>
	struct rs
	{
	    static constexpr size_t SIZE = Op1::SIZE;
	};
    };

    
    template <class... Ops>
    struct RootSum
    {
	static constexpr size_t SIZE = RootSumN<sizeof...(Ops)-1>::template rs<Ops...>::SIZE;
    };

    
    template <typename T, class OpFunction, class... Ops>
    class Operation : public OperationTemplate<T,Operation<T,OpFunction,Ops...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T> OB;
	typedef OperationTemplate<T,Operation<T,OpFunction,Ops...> > OT;
	typedef OpFunction F;
	
	static constexpr size_t SIZE = RootSum<Ops...>::SIZE;

    private:
	std::tuple<Ops...> mOps;

    public:
	typedef decltype(PackNum<sizeof...(Ops)-1>::template mkSteps<Ops...>(0, mOps)) ETuple;
		
	Operation(const Ops&... ops);
	
	template <class ET>
	inline T get(ET pos) const;

	auto rootSteps(std::intptr_t iPtrNum = 0) const // nullptr for simple usage with decltype
	    -> decltype(PackNum<sizeof...(Ops)-1>::mkSteps(iPtrNum, mOps));

	template <class Expr>
	auto loop(Expr exp) const
	    -> decltype(PackNum<sizeof...(Ops)-1>::mkLoop( mOps, exp));
	
    };
    
    template <typename T, class Op, class IndexType>
    class Contraction : public OperationTemplate<T,Contraction<T,Op,IndexType> >
    {
    public:

	typedef T value_type;
	typedef OperationTemplate<T,Contraction<T,Op,IndexType> > OT;

	static constexpr size_t SIZE = Op::SIZE;
	
    private:

	const Op& mOp;
	std::shared_ptr<IndexType> mInd;

    public:
	typedef decltype(mOp.rootSteps(0)) ETuple;
	
	Contraction(const Op& op, std::shared_ptr<IndexType> ind);

	template <class ET>
	inline T get(ET pos) const;

	auto rootSteps(std::intptr_t iPtrNum = 0) const  // nullptr for simple usage with decltype
	    -> decltype(mOp.rootSteps(iPtrNum));

	template <class Expr>
	auto loop(Expr exp) const -> decltype(mInd->iforh(exp));
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

    template <typename T, class OpClass, class... Ranges>
    inline void OperationMaster<T,OpClass,Ranges...>::AssignmentExpr::
    operator()(size_t start, ExtType last) const
    {
	mM.add(start, mSec.template get<ExtType>(last) );
    }
    
    template <typename T, class OpClass, class... Ranges>
    typename OperationMaster<T,OpClass,Ranges...>::AssignmentExpr::ExtType
    OperationMaster<T,OpClass,Ranges...>::AssignmentExpr::
    rootSteps(std::intptr_t iPtrNum) const
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
	mSecond(second), mArrayRef(ma), mDataPtr(mArrayRef.data()),
	mIndex(mkIndex(index)), mIInfo(*mIndex)
    {
	performAssignment(0);
    }

    template <typename T, class OpClass, class... Ranges>
    OperationMaster<T,OpClass,Ranges...>::
    OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OpClass& second,
		    std::shared_ptr<typename CRange::IndexType>& index,
		    const IndexInfo* blockIndex) :
	mSecond(second), mArrayRef(ma), mDataPtr(mArrayRef.data()),
	mIndex(mkIndex(index)), mIInfo(*mIndex)
    {
	performAssignment(0);
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
	AssignmentExpr ae(*this, mSecond); // Expression to be executed within loop
	const auto loop = mSecond.template loop<decltype(mIndex->ifor(ae))>( mIndex->ifor(ae) );
	// hidden Loops outside ! -> auto vectorizable
	loop(); // execute overall loop(s) and so internal hidden loops and so the inherited expressions
    }

    template <typename T, class OpClass, class... Ranges>
    inline T OperationMaster<T,OpClass,Ranges...>::get(size_t pos) const
    {
	return mDataPtr[pos];
    }

   
    /****************************
     *   ConstOperationRoot     *
     ****************************/

    template <typename T, class... Ranges>
    ConstOperationRoot<T,Ranges...>::
    ConstOperationRoot(const MultiArrayBase<T,Ranges...>& ma,
		       const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	mArrayRef(ma), mDataPtr(mArrayRef.data()),
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
    template <class ET>
    inline T ConstOperationRoot<T,Ranges...>::get(ET pos) const
    {
	return mDataPtr[pos.val()];
    }

    template <typename T, class... Ranges>
    MExt<void> ConstOperationRoot<T,Ranges...>::rootSteps(std::intptr_t iPtrNum) const
    {
	return MExt<void>(getStepSize( mIndex->info(), iPtrNum ));
	//return MExt<void>(getStepSize( getRootIndices( mIndex->info() ), iPtrNum ));
    }


    template <typename T, class... Ranges>
    template <class Expr>
    Expr ConstOperationRoot<T,Ranges...>::loop(Expr exp) const
    {
	return exp;
    }
    
    /***********************
     *   OperationRoot     *
     ***********************/
    
    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>::
    OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		  const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	mArrayRef(ma), mDataPtr(mArrayRef.data()),
	mIndex( mkIndex( ma, indices... ) ), mIInfo(*mIndex)
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
	return OperationMaster<T,OpClass,Ranges...>(mArrayRef, in, mIndex);
    }

    template <typename T, class... Ranges>
    template <class ET>
    inline T OperationRoot<T,Ranges...>::get(ET pos) const
    {
	return mDataPtr[pos.val()];
    }

    template <typename T, class... Ranges>
    MExt<void> OperationRoot<T,Ranges...>::rootSteps(std::intptr_t iPtrNum) const
    {
	return MExt<void>(getStepSize( mIndex->info(), iPtrNum ));
	//return MExt<void>(getStepSize( getRootIndices( mIndex->info() ), iPtrNum ));
    }

    template <typename T, class... Ranges>
    template <class Expr>
    Expr OperationRoot<T,Ranges...>::loop(Expr exp) const
    {
	return exp;
    }
    
    /*******************
     *   Operation     *
     *******************/
    
    template <typename T, class OpFunction, class... Ops>
    Operation<T,OpFunction,Ops...>::Operation(const Ops&... ops) :
	mOps(ops...) {}

    template <typename T, class OpFunction, class... Ops>
    template <class ET>
    inline T Operation<T,OpFunction,Ops...>::get(ET pos) const
    {
	typedef std::tuple<Ops...> OpTuple;
	return PackNum<sizeof...(Ops)-1>::
	    template mkOpExpr<SIZE,T,ET,OpTuple,OpFunction>(pos, mOps);
    }

    template <typename T, class OpFunction, class... Ops>
    auto Operation<T,OpFunction,Ops...>::rootSteps(std::intptr_t iPtrNum) const
	-> decltype(PackNum<sizeof...(Ops)-1>::mkSteps(iPtrNum, mOps))
    {
	return PackNum<sizeof...(Ops)-1>::mkSteps(iPtrNum, mOps);
    }

    template <typename T, class OpFunction, class... Ops>
    template <class Expr>
    auto Operation<T,OpFunction,Ops...>::loop(Expr exp) const
	-> decltype(PackNum<sizeof...(Ops)-1>::mkLoop( mOps, exp ))
    {
	return PackNum<sizeof...(Ops)-1>::mkLoop( mOps, exp );
    }

    
    /*********************
     *   Contraction     *
     *********************/

    template <typename T, class Op, class IndexType>
    Contraction<T,Op,IndexType>::Contraction(const Op& op, std::shared_ptr<IndexType> ind) :
	mOp(op),
	mInd(ind) {}

    // forward loop !!!!
    template <typename T, class Op, class IndexType>
    template <class ET>
    inline T Contraction<T,Op,IndexType>::get(ET pos) const
    {
	return mOp.template get<ET>(pos);
    }

    template <typename T, class Op, class IndexType>
    auto Contraction<T,Op,IndexType>::rootSteps(std::intptr_t iPtrNum) const
	-> decltype(mOp.rootSteps(iPtrNum))
    {
	return mOp.rootSteps(iPtrNum);
    }

    template <typename T, class Op, class IndexType>
    template <class Expr>
    auto Contraction<T,Op,IndexType>::loop(Expr exp) const -> decltype(mInd->iforh(exp))
    {
	return mInd->iforh(exp);
    }

}

#endif
