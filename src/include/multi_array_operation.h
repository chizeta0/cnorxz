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

#include "ranges/rheader.h"
#include "pack_num.h"

#include "arith.h"

namespace MultiArrayTools
{

    namespace
    {
	using namespace MultiArrayHelper;
    }

    template <typename T, class OperationClass>
    class OperationBase
    {
    public:
	
	OperationClass& THIS() { return static_cast<OperationClass&>(*this); }
	const OperationClass& THIS() const { return static_cast<OperationClass const&>(*this); }

	template <class Second>
	auto operator+(const OperationBase<T,Second>& in) const
	    -> Operation<T,plus<T>,OperationClass,Second>;
	
	template <class Second>
	auto operator-(const OperationBase<T,Second>& in) const
	    -> Operation<T,minus<T>,OperationClass,Second>;
	
	template <class Second>
	auto operator*(const OperationBase<T,Second>& in) const
	    -> Operation<T,multiplies<T>,OperationClass,Second>;
	
	template <class Second>
	auto operator/(const OperationBase<T,Second>& in) const
	    -> Operation<T,divides<T>,OperationClass,Second>;

	template <class IndexType>
	auto c(const std::shared_ptr<IndexType>& ind) const
	    -> Contraction<T,OperationClass,IndexType>;

	template <class... Indices>
	auto sl(const std::shared_ptr<Indices>&... inds) const
	    -> ConstSlice<T,typename Indices::RangeType...>;

    private:		
	friend OperationClass;
	friend OperationTemplate<T,OperationClass>;
	OperationBase() = default;
    };

    template <typename T, class OperationClass>
    class OperationTemplate : public OperationBase<T,OperationClass>
    {
	/* empty per default; specialize if needed */
    private:
	OperationTemplate() = default;
	friend OperationClass;
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
	//typedef OperationBase<T> OB;
	typedef ContainerRange<T,Ranges...> CRange;
	typedef ContainerIndex<T,typename Ranges::IndexType...> IndexType;
	//typedef typename MultiRange<Ranges...>::IndexType IndexType;
	
	OperationMaster(MutableMultiArrayBase<T,Ranges...>& ma, const OpClass& second,
			IndexType& index);

	OperationMaster(T* data, const OpClass& second,
			IndexType& index);

	inline void set(size_t pos, T val) { mDataPtr[pos] = val; }
	inline void add(size_t pos, T val) { mDataPtr[pos] += val; }
	inline T get(size_t pos) const;

    private:

	void performAssignment(std::intptr_t blockIndexNum);
	OpClass const& mSecond;
	//MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	T* mDataPtr;
	IndexType mIndex;
    };

    
    template <typename T, class... Ranges>
    class ConstOperationRoot : public OperationTemplate<T,ConstOperationRoot<T,Ranges...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T,ConstOperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<T,Ranges...> CRange;
	typedef ContainerIndex<T,typename Ranges::IndexType...> IndexType;

	static constexpr size_t SIZE = 1;
	
	ConstOperationRoot(const MultiArrayBase<T,Ranges...>& ma,
			   const std::shared_ptr<typename Ranges::IndexType>&... indices);

	ConstOperationRoot(std::shared_ptr<MultiArrayBase<T,Ranges...> > maptr,
			   const std::shared_ptr<typename Ranges::IndexType>&... indices);

	ConstOperationRoot(const T* data, const IndexType& ind);

	template <class ET>
	inline T get(ET pos) const;

	MExt<void> rootSteps(std::intptr_t iPtrNum = 0) const; // nullptr for simple usage with decltype

	template <class Expr>
	Expr loop(Expr exp) const;

	const T* data() const;
	
    private:

	//MultiArrayBase<T,Ranges...> const& mArrayRef;
	const T* mDataPtr;
	mutable IndexType mIndex;
	//std::shared_ptr<MultiArrayBase<T,Ranges...> > mMaPtr;
    };

    template <typename T, class Op>
    class StaticCast : public OperationTemplate<T,StaticCast<T,Op> >
    {
    private:
	const Op& mOp;

    public:
	
	typedef T value_type;
	typedef OperationBase<T,StaticCast<T,Op> > OT;
	typedef typename Op::CRange CRange;
	typedef typename Op::IndexType IndexType;

	static constexpr size_t SIZE = Op::SIZE;
	
	StaticCast(const Op& op);

	template <class ET>
	inline T get(ET pos) const;

	auto rootSteps(std::intptr_t iPtrNum = 0) const
	    -> decltype(mOp.rootSteps(iPtrNum));

	template <class Expr>
	Expr loop(Expr exp) const;

    };

    template <typename T, class Op>
    StaticCast<T,Op> staticcast(const Op& op)
    {
	return StaticCast<T,Op>(op);
    }
    
    template <class... Ranges>
    class MetaOperationRoot : public OperationTemplate<std::tuple<typename Ranges::IndexType...>,
						       MetaOperationRoot<Ranges...> >
    {
    public:

	typedef ContainerIndex<std::tuple<typename Ranges::IndexType::MetaType...>,
			       typename Ranges::IndexType...> IndexType;
	typedef typename IndexType::MetaType value_type;
	typedef OperationBase<value_type,MetaOperationRoot<Ranges...> > OT;
	typedef ContainerRange<value_type,Ranges...> CRange;
	
	static constexpr size_t SIZE = 1;
	
	MetaOperationRoot(const IndexType& ind);

	template <class ET>
	inline value_type get(ET pos) const;

	MExt<void> rootSteps(std::intptr_t iPtrNum = 0) const; // nullptr for simple usage with decltype

	template <class Expr>
	Expr loop(Expr exp) const;
		
    private:

	//MultiArrayBase<T,Ranges...> const& mArrayRef;
	//const T* mDataPtr;
	IndexType mIndex;
    };

    template <typename T, class... Ranges>
    class OperationRoot : public OperationTemplate<T,OperationRoot<T,Ranges...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T,OperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<T,Ranges...> CRange;
	typedef ContainerIndex<T,typename Ranges::IndexType...> IndexType;

	static constexpr size_t SIZE = 1;
	
	OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		      const std::shared_ptr<typename Ranges::IndexType>&... indices);

	OperationRoot(T* data, const IndexType& ind);

	template <class OpClass>
	OperationMaster<T,OpClass,Ranges...> operator=(const OpClass& in);

	template <class ET>
	inline T get(ET pos) const;

	MExt<void> rootSteps(std::intptr_t iPtrNum = 0) const; // nullptr for simple usage with decltype

	template <class Expr>
	Expr loop(Expr exp) const;

	T* data() const;
	
	template <class... Indices>
	auto sl(const std::shared_ptr<Indices>&... inds)
	    -> Slice<T,typename Indices::RangeType...>;

    private:

	//MutableMultiArrayBase<T,Ranges...>& mArrayRef;
	T* mDataPtr;
	mutable IndexType mIndex;
    };

    template <typename T>
    class OperationValue : public OperationTemplate<T,OperationValue<T> >
    {
    public:
	typedef T value_type;
	typedef OperationBase<T,OperationValue<T> > OT;
	typedef ContainerRange<T,NullRange> CRange;
	typedef ContainerIndex<T,NullIndex> IndexType;

	static constexpr size_t SIZE = 1;

	OperationValue(const T& val);

	template <class ET>
	inline T get(ET pos) const;

	MExt<void> rootSteps(std::intptr_t iPtrNum = 0) const; // nullptr for simple usage with decltype

	template <class Expr>
	Expr loop(Expr exp) const;
	
    private:
	T mVal;
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
	typedef OperationBase<T,Operation<T,OpFunction,Ops...> > OT;
	typedef OpFunction F;
	
	static constexpr size_t SIZE = RootSum<Ops...>::SIZE;
	static constexpr bool FISSTATIC = OpFunction::FISSTATIC;

    private:
	std::tuple<Ops...> mOps;
	std::shared_ptr<OpFunction> mF; // only if non-static
	
    public:
	typedef decltype(PackNum<sizeof...(Ops)-1>::template mkSteps<Ops...>(0, mOps)) ETuple;
		
	Operation(const Ops&... ops);
	Operation(std::shared_ptr<OpFunction> ff, const Ops&... ops);
	
	template <class ET>
	inline T get(ET pos) const;

	auto rootSteps(std::intptr_t iPtrNum = 0) const // nullptr for simple usage with decltype
	    -> decltype(PackNum<sizeof...(Ops)-1>::mkSteps(iPtrNum, mOps));

	template <class Expr>
	auto loop(Expr exp) const
	    -> decltype(PackNum<sizeof...(Ops)-1>::mkLoop( mOps, exp));
	
    };

    namespace
    {
	template <bool FISSTATIC>
	struct OpMaker
	{
	    template <class OpFunction, class... Ops>
	    static inline auto mkOperation(const std::shared_ptr<OpFunction>& f, const Ops&... ops)
		-> Operation<typename OpFunction::value_type,OpFunction,Ops...>
	    {
		return Operation<typename OpFunction::value_type,OpFunction,Ops...>(f,ops...);
	    }
	};

	template <>
	struct OpMaker<true>
	{
	    template <class OpFunction, class... Ops>
	    static inline auto mkOperation(const std::shared_ptr<OpFunction>& f, const Ops&... ops)
		-> Operation<typename OpFunction::value_type,OpFunction,Ops...>
	    {
		return Operation<typename OpFunction::value_type,OpFunction,Ops...>(ops...);
	    }
	};

    }
    
    template <class OpFunction, class... Ops>
    auto mkOperation(const std::shared_ptr<OpFunction>& f, const Ops&... ops)
	-> Operation<typename OpFunction::value_type,OpFunction,Ops...>
    {
	return OpMaker<OpFunction::FISSTATIC>::mkOperation(f, ops...);
    }


    
    template <typename T, class Op, class IndexType>
    class Contraction : public OperationTemplate<T,Contraction<T,Op,IndexType> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T,Contraction<T,Op,IndexType> > OT;

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
    auto OperationBase<T,OperationClass>::operator+(const OperationBase<T,Second>& in) const
	-> Operation<T,plus<T>,OperationClass,Second>
    {
    	return Operation<T,plus<T>,OperationClass,Second>(THIS(), in.THIS());
    }

    template <typename T, class OperationClass>
    template <class Second>
    auto OperationBase<T,OperationClass>::operator-(const OperationBase<T,Second>& in) const
	-> Operation<T,minus<T>,OperationClass,Second>
    {
    	return Operation<T,minus<T>,OperationClass,Second>(THIS(), in.THIS());
    }
    
    template <typename T, class OperationClass>
    template <class Second>
    auto OperationBase<T,OperationClass>::operator*(const OperationBase<T,Second>& in) const
	-> Operation<T,multiplies<T>,OperationClass,Second>
    {
    	return Operation<T,multiplies<T>,OperationClass,Second>(THIS(), in.THIS());
    }

    template <typename T, class OperationClass>
    template <class Second>
    auto OperationBase<T,OperationClass>::operator/(const OperationBase<T,Second>& in) const
	-> Operation<T,divides<T>,OperationClass,Second>
    {
    	return Operation<T,divides<T>,OperationClass,Second>(THIS(), in.THIS());
    }

    template <typename T, class OperationClass>
    template <class IndexType>
    auto OperationBase<T,OperationClass>::c(const std::shared_ptr<IndexType>& ind) const
	-> Contraction<T,OperationClass,IndexType>
    {
	return Contraction<T,OperationClass,IndexType>(THIS(), ind);
    }

    template <typename T, class OperationClass>
    template <class... Indices>
    auto OperationBase<T,OperationClass>::sl(const std::shared_ptr<Indices>&... inds) const
	-> ConstSlice<T,typename Indices::RangeType...>
    {
	ConstSlice<T,typename Indices::RangeType...> out(inds->range()...);
	out.define(inds...) = *this;
	return out;
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
		    IndexType& index) :
	mSecond(second), mDataPtr(ma.data()),
	mIndex(index)
    {
	performAssignment(0);
    }

    template <typename T, class OpClass, class... Ranges>
    OperationMaster<T,OpClass,Ranges...>::
    OperationMaster(T* data, const OpClass& second,
		    IndexType& index) :
	mSecond(second), mDataPtr(data),
	mIndex(index)
    {
	performAssignment(0);
    }

    template <typename T, class OpClass, class... Ranges>
    void OperationMaster<T,OpClass,Ranges...>::performAssignment(std::intptr_t blockIndexNum)
    {
	AssignmentExpr ae(*this, mSecond); // Expression to be executed within loop
	const auto loop = mSecond.template loop<decltype(mIndex.ifor(ae))>( mIndex.ifor(ae) );
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
	mDataPtr(ma.data()),
	mIndex( ma.begin() )
    {
	//VCHECK(ma.data());
	mIndex(indices...);
    }

    template <typename T, class... Ranges>
    ConstOperationRoot<T,Ranges...>::
    ConstOperationRoot(std::shared_ptr<MultiArrayBase<T,Ranges...> > maptr,
		       const std::shared_ptr<typename Ranges::IndexType>&... indices) :
	mDataPtr(maptr->data()),
	mIndex(maptr->begin())
	//mMaPtr(maptr)
    {
	mIndex(indices...);
    }

    template <typename T, class... Ranges>
    ConstOperationRoot<T,Ranges...>::
    ConstOperationRoot(const T* data, const IndexType& ind) :
	mDataPtr(data),
	mIndex( ind ) { }

    template <typename T, class... Ranges>
    template <class ET>
    inline T ConstOperationRoot<T,Ranges...>::get(ET pos) const
    {
	//VCHECK(pos.val());
	//VCHECK(mDataPtr);
	//VCHECK(mDataPtr[pos.val()])
	return mDataPtr[pos.val()];
    }

    template <typename T, class... Ranges>
    const T* ConstOperationRoot<T,Ranges...>::data() const
    {
	return mDataPtr + mIndex().pos();
    }
    
    template <typename T, class... Ranges>
    MExt<void> ConstOperationRoot<T,Ranges...>::rootSteps(std::intptr_t iPtrNum) const
    {
	return MExt<void>(getStepSize( mIndex, iPtrNum ));
	//return MExt<void>(getStepSize( getRootIndices( mIndex->info() ), iPtrNum ));
    }


    template <typename T, class... Ranges>
    template <class Expr>
    Expr ConstOperationRoot<T,Ranges...>::loop(Expr exp) const
    {
	return exp;
    }

    /********************
     *   StaticCast     *
     ********************/

    template <typename T, class Op>
    StaticCast<T,Op>::StaticCast(const Op& op) : mOp(op) {}

    template <typename T, class Op>
    template <class ET>
    inline T StaticCast<T,Op>::get(ET pos) const
    {
	return static_cast<T>( mOp.get(pos) );
    }

    template <typename T, class Op>
    auto StaticCast<T,Op>::rootSteps(std::intptr_t iPtrNum) const
	-> decltype(mOp.rootSteps(iPtrNum))
    {
	return mOp.rootSteps(iPtrNum);
    }

    template <typename T, class Op>
    template <class Expr>
    Expr StaticCast<T,Op>::loop(Expr exp) const
    {
	return mOp.loop(exp);
    }
    
    
    /****************************
     *   MetaOperationRoot     *
     ****************************/

    template <class... Ranges>
    MetaOperationRoot<Ranges...>::
    MetaOperationRoot(const IndexType& ind) :
	mIndex( ind ) { }

    template <class... Ranges>
    template <class ET>
    inline typename MetaOperationRoot<Ranges...>::value_type
    MetaOperationRoot<Ranges...>::get(ET pos) const
    {
	//VCHECK(pos.val());
	//VCHECK(mDataPtr);
	//VCHECK(mDataPtr[pos.val()])
	return mIndex.meta(pos.val());
    }

    template <class... Ranges>
    MExt<void> MetaOperationRoot<Ranges...>::rootSteps(std::intptr_t iPtrNum) const
    {
	return MExt<void>(getStepSize( mIndex, iPtrNum ));
	//return MExt<void>(getStepSize( getRootIndices( mIndex->info() ), iPtrNum ));
    }


    template <class... Ranges>
    template <class Expr>
    Expr MetaOperationRoot<Ranges...>::loop(Expr exp) const
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
	mDataPtr(ma.data()),
	mIndex( ma.begin() )
    {
	mIndex(indices...);
    }

    template <typename T, class... Ranges>
    OperationRoot<T,Ranges...>::
    OperationRoot(T* data, const IndexType& ind) :
	mDataPtr(data),
	mIndex( ind ) { }

    template <typename T, class... Ranges>
    template <class OpClass>
    OperationMaster<T,OpClass,Ranges...> OperationRoot<T,Ranges...>::operator=(const OpClass& in)
    {
	return OperationMaster<T,OpClass,Ranges...>(mDataPtr, in, mIndex);
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
	return MExt<void>(getStepSize( mIndex, iPtrNum ));
	//return MExt<void>(getStepSize( mIndex.info(), iPtrNum ));
    }

    template <typename T, class... Ranges>
    template <class Expr>
    Expr OperationRoot<T,Ranges...>::loop(Expr exp) const
    {
	return exp;
    }

    template <typename T, class... Ranges>
    T* OperationRoot<T,Ranges...>::data() const
    {
	return mDataPtr + mIndex().pos();
    }

    template <typename T, class... Ranges>
    template <class... Indices>
    auto OperationRoot<T,Ranges...>::sl(const std::shared_ptr<Indices>&... inds)
	-> Slice<T,typename Indices::RangeType...>
    {
	Slice<T,typename Indices::RangeType...> out(inds->range()...);
	out.define(inds...) = *this;
	return out;
    }


    /************************
     *   OperationValue     *
     ************************/

    template <typename T>
    OperationValue<T>::OperationValue(const T& val) : mVal(val) {}

    template <typename T>
    template <class ET>
    inline T OperationValue<T>::get(ET pos) const
    {
	return mVal;
    }

    template <typename T>
    MExt<void> OperationValue<T>::rootSteps(std::intptr_t iPtrNum) const
    {
	return MExt<void>(0);
    }

    template <typename T>
    template <class Expr>
    Expr OperationValue<T>::loop(Expr exp) const
    {
	return exp;
    }

    
    /*******************
     *   Operation     *
     *******************/
    
    template <typename T, class OpFunction, class... Ops>
    Operation<T,OpFunction,Ops...>::Operation(const Ops&... ops) :
	mOps(ops...)
    {
	static_assert( FISSTATIC, "need function instance for non-static function" );
    }

    template <typename T, class OpFunction, class... Ops>
    Operation<T,OpFunction,Ops...>::Operation(std::shared_ptr<OpFunction> ff,
					      const Ops&... ops) :
	mOps(ops...),
	mF(ff)
    {
	static_assert( not FISSTATIC, "using instance of function supposed to be static" );
    }

    template <typename T, class OpFunction, class... Ops>
    template <class ET>
    inline T Operation<T,OpFunction,Ops...>::get(ET pos) const
    {
	typedef std::tuple<Ops...> OpTuple;
	return PackNum<sizeof...(Ops)-1>::
	    template mkOpExpr<SIZE,T,ET,OpTuple,OpFunction>(mF, pos, mOps);
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

#include "type_operations.h"

#endif
