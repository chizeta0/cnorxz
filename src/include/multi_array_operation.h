// -*- C++ -*-

#ifndef __multi_array_operation_h__
#define __multi_array_operation_h__

#include <cstdlib>
#include <tuple>
#include <cmath>
#include <map>
#include <utility>
#include <type_traits>

#include "base_def.h"
#include "mbase_def.h"

#include "ranges/rheader.h"

#include "arith.h"
#include "xfor/xfor.h"
#include "type_operations.h"

#include "statics/static_for.h"

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

        template <typename U, class Second>
	auto operator+(const OperationBase<U,Second>& in) const;
	
        template <typename U, class Second>
	auto operator-(const OperationBase<U,Second>& in) const;
	
        template <typename U, class Second>
	auto operator*(const OperationBase<U,Second>& in) const;
	
        template <typename U, class Second>
	auto operator/(const OperationBase<U,Second>& in) const;

	template <class IndexType>
	auto c(const std::shared_ptr<IndexType>& ind) const
	    -> Contraction<T,OperationClass,IndexType>;

	template <class... Indices>
	auto sl(const std::shared_ptr<Indices>&... inds) const
	    -> ConstSlice<T,typename Indices::RangeType...>;

	template <class... Indices>
	auto slc(const std::shared_ptr<Indices>&... inds) const
	    -> SliceContraction<T,OperationClass,Indices...>;

	template <class... Indices>
	auto p(const std::shared_ptr<Indices>&... inds) const
	    -> ConstOperationRoot<T,typename Indices::RangeType...>;

	template <class... Indices>
	auto to(const std::shared_ptr<Indices>&... inds) const
	    -> MultiArray<T,typename Indices::RangeType...>;

        template <class... Indices>
	auto addto(const std::shared_ptr<Indices>&... inds) const
	    -> MultiArray<T,typename Indices::RangeType...>;

        template <class... Indices>
	auto pto(const std::shared_ptr<Indices>&... inds) const
	    -> MultiArray<T,typename Indices::RangeType...>;

        template <class... Indices>
	auto paddto(const std::shared_ptr<Indices>&... inds) const
	    -> MultiArray<T,typename Indices::RangeType...>;

        template <typename R, class... Args> // Args = Operation Classes
        auto a(const std::shared_ptr<function<R,T,typename Args::value_type...>>& ll, const Args&... args) const
            -> Operation<R,function<R,T,typename Args::value_type...>,OperationClass, Args...>;

	auto ptr() const
	    -> OperationPointer<T,OperationClass>;
	
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

    template <size_t N>
    struct RootSumN
    {

	template <class... Exprs>
	static inline auto rootSteps(const std::tuple<Exprs...>& etp, std::intptr_t i)
	{
	    return RootSumN<N-1>::rootSteps(etp,i).extend( std::get<N>(etp).rootSteps(i) );
	}

	template <class ExtType, class... Exprs>
	static inline void exec( size_t start, ExtType last, std::tuple<Exprs...>& etp)
	{
	    std::get<sizeof...(Exprs)-N-1>(etp)(start,last);
	    RootSumN<N-1>::exec(start,last.next(),etp);
	}

	template <class ExtType, class... Exprs>
	static inline size_t get( ExtType last, const std::tuple<Exprs...>& etp)
	{
	    std::get<sizeof...(Exprs)-N-1>(etp).get(last);
	    return RootSumN<N-1>::get(last.next(),etp);
	}

	template <class ExtType, class... Exprs>
	static inline void set( ExtType last, std::tuple<Exprs...>& etp)
	{
	    std::get<sizeof...(Exprs)-N-1>(etp).set(last);
	    RootSumN<N-1>::set(last.next(),etp);
	}
    };

    template <>
    struct RootSumN<0>
    {

	template <class... Exprs>
	static inline auto rootSteps(const std::tuple<Exprs...>& etp, std::intptr_t i)
	{
	    return std::get<0>(etp).rootSteps(i);
	}

	template <class ExtType, class... Exprs>
	static inline void exec( size_t start, ExtType last, std::tuple<Exprs...>& etp)
	{
	    std::get<sizeof...(Exprs)-1>(etp)(start,last);
	}

	template <class ExtType, class... Exprs>
	static inline size_t get( ExtType last, const std::tuple<Exprs...>& etp)
	{
	    std::get<sizeof...(Exprs)-1>(etp).get(last);
	    return 0;
	}

	template <class ExtType, class... Exprs>
	static inline void set( ExtType last, std::tuple<Exprs...>& etp)
	{
	    std::get<sizeof...(Exprs)-1>(etp).set(last);
	}
    };

    
    template <typename T>
    struct SelfIdentity
    {
        static inline T& sapply(T& a, T b)
        {
            return a = b;
        }
    };

    enum class OpIndexAff {
                           EXTERN = 0,
                           TARGET = 1
    };

    template <class T>
    struct VType
    {
	typedef T type;
	static constexpr size_t MULT = sizeof(type)/sizeof(T);
    };

    template <>
    struct VType<double>
    {
	typedef v256 type;
	static constexpr size_t MULT = sizeof(type)/sizeof(double);
    };

    template <template <typename...> class F,typename... Ts>
    inline auto mkVFuncPtr(const std::shared_ptr<F<Ts...>>& f)
    {
	return std::shared_ptr<F<typename VType<Ts>::type...>>();
	// empty, implement corresponding constructors...!!!
    }

    template <template <typename...> class F,typename... Ts>
    inline auto mkVFunc(const F<Ts...>& f)
    {
	return F<typename VType<Ts>::type...>();
	// empty, implement corresponding constructors...!!!
    }

    template <class F>
    using VFunc = decltype(mkVFunc(std::declval<F>()));

    template <typename T, class F>
    struct IAccess
    {
	typedef T value_type;
	typedef T in_type;
	static constexpr size_t VSIZE = sizeof(value_type) / sizeof(in_type);

	template <typename Op, class ExtType>
	static inline void f(T*& t, size_t pos, const Op& op, ExtType e)
	{
	    F::selfApply(t[pos],op.get(e));
	}
    };

    template <typename T, class F>
    struct IVAccess
    {
	typedef typename VType<T>::type value_type;
	typedef T in_type;
	static constexpr size_t VSIZE = sizeof(value_type) / sizeof(in_type);

	template <typename Op, class ExtType>
	static inline void f(T*& t, size_t pos, const Op& op, ExtType e)
	{
	    //VCHECK(pos);
	    VFunc<F>::selfApply(*reinterpret_cast<value_type*>(t+pos),op.template vget<value_type>(e));
	}
    };

    template <typename T>
    using xxxplus = plus<T>;
    
    template <typename T>
    using IAssign = IAccess<T,identity<T>>;

    template <typename T>
    using IPlus = IAccess<T,plus<T>>;

    template <typename T>
    using IVAssign = IVAccess<T,identity<T>>;

    template <typename T>
    using IVPlus = IVAccess<T,plus<T>>;
    
    template <typename T, class IOp, class Target, class OpClass, OpIndexAff OIA=OpIndexAff::EXTERN>
    class AssignmentExpr : public ExpressionBase
    {
    private:
        AssignmentExpr() = default;

        Target mTar;
        OpClass mSec;
        T* mDataPtr;
            
    public:

        static constexpr size_t LAYER = 0;
        static constexpr size_t NHLAYER = 0;
        static constexpr size_t SIZE = Target::SIZE + OpClass::SIZE;
        typedef decltype(mTar.rootSteps(0).extend( mSec.rootSteps(0) )) ExtType;
	    
        AssignmentExpr(T* dataPtr, const Target& tar, const OpClass& sec);
        AssignmentExpr(const AssignmentExpr& in) = default;
        AssignmentExpr(AssignmentExpr&& in) = default;
        AssignmentExpr& operator=(const AssignmentExpr& in) = default;
        AssignmentExpr& operator=(AssignmentExpr&& in) = default;

	virtual std::shared_ptr<ExpressionBase> deepCopy() const override final
	{
	    return std::make_shared<AssignmentExpr<T,IOp,Target,OpClass,OIA>>(*this);
	}
	
        inline void operator()(size_t start = 0); 
        inline void operator()(size_t start, ExtType last);
        auto rootSteps(std::intptr_t iPtrNum = 0) const -> ExtType;

        inline void operator()(size_t mlast, DExt last) override final;

        inline DExt dRootSteps(std::intptr_t iPtrNum = 0) const override final;
        inline DExt dExtension() const override final;
    };

    template <typename T, class Target, class OpClass, OpIndexAff OIA=OpIndexAff::EXTERN>
    using AssignmentExpr2 = AssignmentExpr<T,IAssign<T>,Target,OpClass,OIA>;

    template <typename T, class Target, class OpClass, OpIndexAff OIA=OpIndexAff::EXTERN>
    using AddExpr = AssignmentExpr<T,IPlus<T>,Target,OpClass,OIA>;

    template <class... Ops>
    struct OperationTuple
    {
	OperationTuple(const Ops&... ops) : mOps(ops...) {}
	std::tuple<Ops...> mOps;
	auto rootSteps(std::intptr_t iPtrNum) const;
    };

    template <class... Ops>
    auto OperationTuple<Ops...>::rootSteps(std::intptr_t iPtrNum) const
    {
	return MA_SCFOR(i,0,sizeof...(Ops),i+1,std::get<i>(mOps).rootSteps(iPtrNum),extend);
    }
    
    template <typename T, class... Ops>
    class MOp
    {
    private:
	MOp() = default;
	OperationTuple<Ops...> mOps;
	
    public:
        static constexpr size_t LAYER = 0;
	static constexpr size_t NHLAYER = 0;
	static constexpr size_t SIZE = (... + Ops::SIZE);
	typedef decltype(mOps.rootSteps(0)) ExtType;
	
	MOp(const Ops&... exprs);
	
	MOp(const MOp& in) = default;
	MOp(MOp&& in) = default;
	MOp& operator=(const MOp& in) = default;
	MOp& operator=(MOp&& in) = default;

        inline size_t get(ExtType last) const;

	template <typename V>
	inline size_t vget(ExtType last) const { return get(last); }
	
	inline MOp& set(ExtType last);
        auto rootSteps(std::intptr_t iPtrNum = 0) const -> ExtType;

	template <class Expr>
	auto loop(Expr exp) const;

	T* data() const { assert(0); return nullptr; }

    };

    
    template <class OpClass, class NextExpr>
    class GetExpr : public ExpressionBase
    {
    private:
        GetExpr() = default;

        OpClass mSec;
        NextExpr mNExpr;
            
    public:

        static constexpr size_t LAYER = 0;
	static constexpr size_t NHLAYER = 0;
        static constexpr size_t SIZE = OpClass::SIZE + NextExpr::SIZE;
        typedef decltype(mSec.rootSteps(0).extend( mNExpr.rootSteps(0) ) ) ExtType;
	    
        GetExpr(const OpClass& sec, const NextExpr& nexpr);
        GetExpr(const GetExpr& in) = default;
        GetExpr(GetExpr&& in) = default;
        GetExpr& operator=(const GetExpr& in) = default;
        GetExpr& operator=(GetExpr&& in) = default;
	    
	virtual std::shared_ptr<ExpressionBase> deepCopy() const override final
	{
	    return std::make_shared<GetExpr<OpClass,NextExpr>>(*this);
	}

        inline void operator()(size_t start = 0);
        inline void get(ExtType last);

	template <typename V>
        inline void vget(ExtType last) { get(last); }
	
        inline void operator()(size_t start, ExtType last);
        auto rootSteps(std::intptr_t iPtrNum = 0) const -> ExtType;

        inline void operator()(size_t mlast, DExt last) override final;

        inline DExt dRootSteps(std::intptr_t iPtrNum = 0) const override final;
        inline DExt dExtension() const override final;
    };

    template <class OpClass, class NextExpr>
    auto mkGetExpr(const OpClass& op, const NextExpr& nexpr)
    {
        return GetExpr<OpClass,NextExpr>(op, nexpr);
    }

    template <typename T, class... Ops>
    auto mkMOp(const Ops&... exprs)
    {
        return MOp<T,Ops...>(exprs...);
    }

    template <typename T, class... Ranges>
    class ConstOperationRoot : public OperationTemplate<T,ConstOperationRoot<T,Ranges...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T,ConstOperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<Ranges...> CRange;
	typedef ConstContainerIndex<T,typename Ranges::IndexType...> IndexType;

	static constexpr size_t SIZE = 1;
        static constexpr bool CONT = true;
        static constexpr bool VABLE = true;
	
	ConstOperationRoot(const MultiArrayBase<T,Ranges...>& ma,
			   const std::shared_ptr<typename Ranges::IndexType>&... indices);

	ConstOperationRoot(std::shared_ptr<MultiArrayBase<T,Ranges...> > maptr,
			   const std::shared_ptr<typename Ranges::IndexType>&... indices);

	ConstOperationRoot(const T* data, const IndexType& ind);

	template <class ET>
	inline const T& get(ET pos) const;

	template <typename V, class ET>
	inline const V& vget(ET pos) const;
	
	template <class ET>
	inline ConstOperationRoot& set(ET pos);

	MExt<None> rootSteps(std::intptr_t iPtrNum = 0) const; // nullptr for simple usage with decltype

	template <class Expr>
	Expr loop(Expr exp) const;

	const T* data() const;
	
    private:

	const T* mDataPtr;
        const T* mOrigDataPtr;
	IndexType mIndex;
	std::shared_ptr<MultiArrayBase<T,Ranges...> > mMaPtr; // never remove this ptr, otherwise we lose temporary container instances!
    };

    template <typename T, class Op>
    class StaticCast : public OperationTemplate<T,StaticCast<T,Op> >
    {
    private:
	Op mOp;

    public:
	
	typedef T value_type;
	typedef OperationBase<T,StaticCast<T,Op> > OT;
	typedef typename Op::CRange CRange;
	typedef typename Op::IndexType IndexType;

	static constexpr size_t SIZE = Op::SIZE;
        static constexpr bool CONT = false;
        static constexpr bool VABLE = false;
	
	StaticCast(const Op& op);

	template <class ET>
	inline T get(ET pos) const;

	template <typename V, class ET>
	inline V vget(ET pos) const;

	template <class ET>
	inline StaticCast& set(ET pos);

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
    
    template <class Range>
    class MetaOperationRoot : public OperationTemplate<typename Range::MetaType,
						       MetaOperationRoot<Range> >
    {
    public:

	typedef typename Range::IndexType IndexType;
	typedef typename IndexType::MetaType value_type;
	typedef OperationBase<value_type,MetaOperationRoot<Range> > OT;
	
	static constexpr size_t SIZE = 1;
        static constexpr bool CONT = false;
        static constexpr bool VABLE = false;
        
	MetaOperationRoot(const std::shared_ptr<IndexType>& ind);

	template <class ET>
	inline value_type get(ET pos) const;

	template <typename V, class ET>
	inline V vget(ET pos) const;

	template <class ET>
	inline MetaOperationRoot& set(ET pos);

	MExt<None> rootSteps(std::intptr_t iPtrNum = 0) const; // nullptr for simple usage with decltype

	template <class Expr>
	Expr loop(Expr exp) const;
		
    private:

        mutable IndexType mWorkIndex;
        std::shared_ptr<IndexType> mIndex;
    };

    template <typename T, class... Ranges>
    class OperationRoot : public OperationTemplate<T,OperationRoot<T,Ranges...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T,OperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<Ranges...> CRange;
	typedef ConstContainerIndex<T,typename Ranges::IndexType...> IndexType;

	static constexpr size_t SIZE = 1;
        static constexpr bool CONT = true;
        static constexpr bool VABLE = true;

    private:

	T* mDataPtr;
        T* mOrigDataPtr;
	IndexType mIndex;

    public:
	OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		      const std::shared_ptr<typename Ranges::IndexType>&... indices);

	OperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
		      const std::tuple<std::shared_ptr<typename Ranges::IndexType>...>& indices);

	OperationRoot(T* data, const IndexType& ind);

        template <class IOp, class OpClass>
        auto asx(const OpClass& in) const
            -> decltype(mIndex.ifor(1,in.loop(AssignmentExpr<T,IOp,OperationRoot<T,Ranges...>,OpClass,OpIndexAff::TARGET>
                                              (mOrigDataPtr,*this,in))).template vec<IOp::VSIZE>());

        template <class IOp, class OpClass>
        auto asxExpr(const OpClass& in) const
            -> decltype(in.loop(AssignmentExpr<T,IOp,OperationRoot<T,Ranges...>,OpClass>(mOrigDataPtr,*this,in)));
            
        template <class IOp, class OpClass, class Index>
        auto asx(const OpClass& in, const std::shared_ptr<Index>& i) const
            -> decltype(i->ifor(1,in.loop(AssignmentExpr<T,IOp,OperationRoot<T,Ranges...>,OpClass>
                                          (mOrigDataPtr,*this,in))).template vec<IOp::VSIZE>());
            
        template <class OpClass>
        auto assign(const OpClass& in) const
	    -> decltype(this->template asx<IAssign<T>>(in));

	template <class OpClass>
        auto assignExpr(const OpClass& in) const
	    -> decltype(this->template asxExpr<IAssign<T>>(in));
	    
        template <class OpClass, class Index>
        auto assign(const OpClass& in, const std::shared_ptr<Index>& i) const
	    -> decltype(this->template asx<IAssign<T>>(in,i));
	
	template <class OpClass>
        auto plus(const OpClass& in) const
	    -> decltype(this->template asx<IPlus<T>>(in));

        template <class OpClass, class Index>
        auto plus(const OpClass& in, const std::shared_ptr<Index>& i) const
	    -> decltype(this->template asx<IPlus<T>>(in,i));
            
        template <class OpClass>
        OperationRoot& operator=(const OpClass& in);

        template <class OpClass>
        OperationRoot& operator+=(const OpClass& in);

        OperationRoot& operator=(const OperationRoot& in);
        
        ParallelOperationRoot<T,Ranges...> par();
        
	template <class ET>
	inline T& get(ET pos) const;

	template <typename V, class ET>
	inline V& vget(ET pos) const;
	
	template <class ET>
	inline OperationRoot& set(ET pos);

	MExt<None> rootSteps(std::intptr_t iPtrNum = 0) const; // nullptr for simple usage with decltype

	template <class Expr>
	Expr loop(Expr exp) const;
        
	T* data() const;
	
	template <class... Indices>
	auto sl(const std::shared_ptr<Indices>&... inds)
	    -> Slice<T,typename Indices::RangeType...>;

    };

    template <typename T, class... Ranges>
    class ParallelOperationRoot : public OperationTemplate<T,ParallelOperationRoot<T,Ranges...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T,ParallelOperationRoot<T,Ranges...> > OT;
	typedef ContainerRange<Ranges...> CRange;
	typedef ConstContainerIndex<T,typename Ranges::IndexType...> IndexType;

	static constexpr size_t SIZE = 1;
        static constexpr bool CONT = true;
        static constexpr bool VABLE = true;

    private:

	T* mDataPtr;
        T* mOrigDataPtr;
	IndexType mIndex;

    public:
	ParallelOperationRoot(MutableMultiArrayBase<T,Ranges...>& ma,
                              const std::shared_ptr<typename Ranges::IndexType>&... indices);

	ParallelOperationRoot(T* data, const IndexType& ind);

        template <class IOp, class OpClass>
        auto asx(const OpClass& in) const
            -> decltype(mIndex.pifor(1,in.loop(AssignmentExpr<T,IOp,ParallelOperationRoot<T,Ranges...>,OpClass,OpIndexAff::TARGET>
                                              (mOrigDataPtr,*this,in))).template vec<IOp::VSIZE>());

        template <class IOp, class OpClass>
        auto asxExpr(const OpClass& in) const
            -> decltype(in.loop(AssignmentExpr<T,IOp,ParallelOperationRoot<T,Ranges...>,OpClass>(mOrigDataPtr,*this,in)));
            
        template <class IOp, class OpClass, class Index>
        auto asx(const OpClass& in, const std::shared_ptr<Index>& i) const
            -> decltype(i->pifor(1,in.loop(AssignmentExpr<T,IOp,ParallelOperationRoot<T,Ranges...>,OpClass>
                                          (mOrigDataPtr,*this,in))).template vec<IOp::VSIZE>());
            
        template <class OpClass>
        auto assign(const OpClass& in) const
	    -> decltype(this->template asx<IAssign<T>>(in));

	template <class OpClass>
        auto assignExpr(const OpClass& in) const
	    -> decltype(this->template asxExpr<IAssign<T>>(in));
	    
        template <class OpClass, class Index>
        auto assign(const OpClass& in, const std::shared_ptr<Index>& i) const
	    -> decltype(this->template asx<IAssign<T>>(in,i));
	
	template <class OpClass>
        auto plus(const OpClass& in) const
	    -> decltype(this->template asx<IPlus<T>>(in));

        template <class OpClass, class Index>
        auto plus(const OpClass& in, const std::shared_ptr<Index>& i) const
	    -> decltype(this->template asx<IPlus<T>>(in,i));

        template <class OpClass>
        ParallelOperationRoot& operator=(const OpClass& in);

        template <class OpClass>
        ParallelOperationRoot& operator+=(const OpClass& in);

        ParallelOperationRoot& operator=(const ParallelOperationRoot& in);
        
	template <class ET>
	inline T& get(ET pos) const;
	 
	template <typename V, class ET>
	inline V& vget(ET pos) const;

	template <class ET>
	inline ParallelOperationRoot& set(ET pos);

	MExt<None> rootSteps(std::intptr_t iPtrNum = 0) const; // nullptr for simple usage with decltype

	template <class Expr>
	Expr loop(Expr exp) const;
        
	T* data() const;

    };

    template <typename T>
    class OperationValue : public OperationTemplate<T,OperationValue<T> >
    {
    public:
	typedef T value_type;
	typedef OperationBase<T,OperationValue<T> > OT;
	typedef ContainerRange<NullRange> CRange;
	typedef ConstContainerIndex<T,NullIndex> IndexType;

	static constexpr size_t SIZE = 0;
        static constexpr bool CONT = true;
        static constexpr bool VABLE = false;

	OperationValue(const T& val);

	template <class ET>
	inline const T& get(ET pos) const;

	template <typename V, class ET>
	inline V vget(ET pos) const;

	template <class ET>
	inline OperationValue& set(ET pos);

	None rootSteps(std::intptr_t iPtrNum = 0) const; // nullptr for simple usage with decltype

	template <class Expr>
	Expr loop(Expr exp) const;
	
    private:
	T mVal;
    };
    
    template <typename T, class Op>
    class OperationPointer : public OperationTemplate<const T*,OperationPointer<T,Op>>
    {
    public:
	typedef T value_type;
	typedef OperationTemplate<const T*,OperationPointer<T,Op>> OT;
	
	static constexpr size_t SIZE = Op::SIZE;
        static constexpr bool CONT = false;

    private:
	Op mOp;

    public:

	OperationPointer(const Op& op) : mOp(op)
	{
	    static_assert(Op::CONT,
			  "OperationPointer can only be applied to containing operations");
	}
	
	template <class ET>
	inline const T* get(ET pos) const;

	template <class ET>
	inline OperationPointer& set(ET pos);

	auto rootSteps(std::intptr_t iPtrNum = 0) const // nullptr for simple usage with decltype
	    -> decltype(mOp.rootSteps(0));

	template <class Expr>
	auto loop(Expr exp) const
	    -> decltype(mOp.loop(exp));

	T const** data() const { assert(0); return nullptr; }
    };

    /*
    template <typename T>
    struct TFold
    {
	TFold() = default;
	TFold(const TFold& in) = default;
	TFold& operator=(const TFold& in) = default;
	TFold(TFold&& in) = default;
	TFold& operator=(TFold&& in) = default;
	explicit TFold(const T v) : val(v) {}
	
	T val;
	inline T& operator*() { return val; }
	inline const T& operator*() const { return val; }
    };

    template <typename T1, typename T2>
    inline std::tuple<TFold<T1>,TFold<T2>> operator,(const TFold<T1>& a1, const TFold<T2>& a2)
    {
	return std::make_tuple(a1,a2);
    }

    template <typename T1, typename T2...>
    inline std::tuple<TFold<T1>,TFold<T2>> operator,(const TFold<T1>& a1,
						     const std::tuple<TFold<T2>...>& a2)
    {
	return std::tuple_cat(std::make_tuple(a1),a2);
    }

    template <typename T2, typename T1...>
    inline std::tuple<TFold<T1>,TFold<T2>> operator,(const std::tuple<TFold<T1>...>& a1,
						     const TFold<T2>& a2)
    {
	return std::tuple_cat(a1,std::make_tuple(a2));
    }
    */
    template <typename T, class OpFunction, class... Ops>
    class Operation : public OperationTemplate<T,Operation<T,OpFunction,Ops...> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T,Operation<T,OpFunction,Ops...> > OT;
	typedef OpFunction F;
	
	static constexpr size_t SIZE = (... + Ops::SIZE);
	static constexpr bool FISSTATIC = OpFunction::FISSTATIC;
        static constexpr bool CONT = false;
	static constexpr bool VABLE =
	    (... and (Ops::VABLE and std::is_same<T,typename Ops::value_type>::value));

    private:
	OperationTuple<Ops...> mOps;
	std::shared_ptr<OpFunction> mF; // only if non-static
	typedef decltype(mOps.rootSteps(0)) ExtType;
	
    public:
		
	Operation(const Ops&... ops);
	Operation(std::shared_ptr<OpFunction> ff, const Ops&... ops);

	template <class ET>
	inline auto get(ET pos) const;

	template <typename V, class ET>
	inline auto vget(ET pos) const;

	template <class ET>
	inline Operation& set(ET pos);
	
	auto rootSteps(std::intptr_t iPtrNum = 0) const
	    -> ExtType; // nullptr for simple usage with decltype

	template <class Expr>
	auto loop(Expr exp) const;

	T* data() const { assert(0); return nullptr; }
    };
    
    template <class OpFunction, class... Ops>
    auto mkOperation(const std::shared_ptr<OpFunction>& f, const Ops&... ops)
	-> Operation<typename OpFunction::value_type,OpFunction,Ops...>
    {
	if constexpr(OpFunction::FISSTATIC){
	    return Operation<typename OpFunction::value_type,OpFunction,Ops...>(ops...);
	}
	else {
	    return Operation<typename OpFunction::value_type,OpFunction,Ops...>(f,ops...);
	}
    }
    
    template <typename T, class Op, class IndexType>
    class Contraction : public OperationTemplate<T,Contraction<T,Op,IndexType> >
    {
    public:

	typedef T value_type;
	typedef OperationBase<T,Contraction<T,Op,IndexType> > OT;

	static constexpr size_t SIZE = Op::SIZE;
        static constexpr bool CONT = Op::CONT;
        static constexpr bool VABLE = Op::VABLE;
        
    private:

	Op mOp;
	std::shared_ptr<IndexType> mInd;

    public:
	typedef decltype(mOp.rootSteps(0)) ETuple;
	
	Contraction(const Op& op, std::shared_ptr<IndexType> ind);

	template <class ET>
	inline auto get(ET pos) const
            -> decltype(mOp.template get<ET>(pos));

	template <typename V, class ET>
	inline auto vget(ET pos) const
            -> decltype(mOp.template vget<V,ET>(pos));

	template <class ET>
	inline Contraction& set(ET pos);

	T* data() const { assert(0); return nullptr; }

	auto rootSteps(std::intptr_t iPtrNum = 0) const  // nullptr for simple usage with decltype
	    -> decltype(mOp.rootSteps(iPtrNum));

	template <class Expr>
	auto loop(Expr exp) const
	    -> decltype(mInd->iforh(1,mOp.loop(exp)));
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
        static constexpr bool CONT = false;
	static constexpr bool VABLE = false;

    private:

	mutable Op mOp;
	mutable std::shared_ptr<MultiArray<T,typename Indices::RangeType...> > mCont;
	mutable OperationRoot<T,typename Indices::RangeType...> mTarOp;
		
    public:
	typedef decltype(mOp.rootSteps(0)) ETuple;
	
	SliceContraction(const Op& op, std::shared_ptr<Indices>... ind);
	
	template <class ET>
	inline const value_type& get(ET pos) const;

	template <class ET>
	inline SliceContraction& set(ET pos);

	auto rootSteps(std::intptr_t iPtrNum = 0) const  // nullptr for simple usage with decltype
	    -> decltype(mOp.rootSteps(iPtrNum));

	template <class Expr>
	auto loop(Expr exp) const -> decltype(mOp.loop(exp)); // no loop
	
    };	
}


#include "type_operations.h"

#endif
