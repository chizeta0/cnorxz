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

	template <class... Indices>
	auto slc(const std::shared_ptr<Indices>&... inds) const
	    -> SliceContraction<T,OperationClass,Indices...>;
	
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

	template <class ET>
	inline const ConstOperationRoot& set(ET pos) const;

	MExt<void> rootSteps(std::intptr_t iPtrNum = 0) const; // nullptr for simple usage with decltype

	template <class Expr>
	Expr loop(Expr exp) const;

	const T* data() const;
	
    private:

	//MultiArrayBase<T,Ranges...> const& mArrayRef;
	const T* mDataPtr;
	mutable IndexType mIndex;
	mutable size_t mOff = 0;
	std::shared_ptr<MultiArrayBase<T,Ranges...> > mMaPtr; // never remove this ptr, otherwise we lose temporary container instances!
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

	template <class ET>
	inline const StaticCast& set(ET pos) const;

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

	template <class ET>
	inline const MetaOperationRoot& set(ET pos) const;

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

	OperationMaster<T,OperationRoot,Ranges...> operator=(const OperationRoot& in);
	
	template <class ET>
	inline T get(ET pos) const;

	template <class ET>
	inline const OperationRoot& set(ET pos) const;

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
 	mutable size_t mOff = 0;
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

	template <class ET>
	inline const OperationValue& set(ET pos) const;

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

	template <class ET>
	inline const Operation& set(ET pos) const;

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

	template <class ET>
	inline const Contraction& set(ET pos) const;

	auto rootSteps(std::intptr_t iPtrNum = 0) const  // nullptr for simple usage with decltype
	    -> decltype(mOp.rootSteps(iPtrNum));

	template <class Expr>
	auto loop(Expr exp) const -> decltype(mInd->iforh(1,exp));
    };

    template <typename T, class Op, class... Indices>
    //    class SliceContraction : public OperationTemplate
    //<MultiArray<T,typename Indices::RangeType...>,
    //SliceContraction<MultiArray<T,typename Indices::RangeType...>,Op,Indices...> >
    class SliceContraction : public OperationTemplate<T,SliceContraction<T,Op,Indices...> >
    {
    public:
	typedef MultiArray<T,typename Indices::RangeType...> value_type;
	typedef OperationTemplate<T,SliceContraction<T,Op,Indices...> > OT;

	static constexpr size_t SIZE = Op::SIZE;

    private:

	const Op& mOp;
	mutable std::shared_ptr<MultiArray<T,typename Indices::RangeType...> > mCont;
	mutable OperationRoot<T,typename Indices::RangeType...> mTarOp;
		
    public:
	typedef decltype(mOp.rootSteps(0)) ETuple;
	
	SliceContraction(const Op& op, std::shared_ptr<Indices>... ind);
	
	template <class ET>
	inline const value_type& get(ET pos) const;

	template <class ET>
	inline const SliceContraction& set(ET pos) const;

	auto rootSteps(std::intptr_t iPtrNum = 0) const  // nullptr for simple usage with decltype
	    -> decltype(mOp.rootSteps(iPtrNum));

	template <class Expr>
	auto loop(Expr exp) const -> decltype(mOp.loop(exp)); // no loop
	
    };	
}


#include "type_operations.h"

#endif
