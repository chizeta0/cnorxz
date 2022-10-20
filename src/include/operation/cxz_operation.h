// -*- C++ -*-

#ifndef __cxz_operation_h__
#define __cxz_operation_h__

#include "base/base.h"

#include "arith.h"
#include "type_operations.h"
#include "op_expressions.h"
#include "access.h"

#include "statics/static_for.h"

namespace CNORXZ
{


    template <class OpT>
    class COpInterface
    {
    public:
	
	OpT& THIS() { return static_cast<OpT&>(*this); }
	const OpT& THIS() const { return static_cast<const OpT&>(*this); }

	template <class IndexT>
	constexpr auto c(const Sptr<IndexT>& ind) const;

	template <class F, class... Args>
	constexpr auto o(F&& f, Args&&... args) const;

	template <class PosT>
	constexpr decltype(auto) get(const PosT& pos) const
	{ return THIS().get(pos); }

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const
	{ return THIS().rootSteps(id); }
	
	template <class Xpr>
	constexpr decltype(auto) loop(Xpr&& exp) const
	{ return THIS().loop(exp); }
    };

    template <class OpT>
    class OpInterface : public COpInterface<OpT>
    {
    public:
	typedef COpInterface<OpT> OI;

	OpT& THIS() { return static_cast<OpT&>(*this); }
	const OpT& THIS() const { return static_cast<const OpT&>(*this); }

	template <class IndexT, class F, class... Args>
	constexpr decltype(auto) ax(const Sptr<IndexT>& ind, F&& f, Args&&... args);

	template <class IndexT, class F, class... Args>
	inline SizeT a(const Sptr<IndexT>& ind, F&& f, Args&&... args);
    };

    template <class T>
    struct is_operation
    { static constexpr bool value = std::is_base_of<COpInterface,T>::value };

    template <class T>
    struct is_mutable_operation
    { static constexpr bool value = std::is_base_of<OpInterface,T>::value };

    template <typename T, class IndexT>
    class COpRoot : public COpInterface<T,COpRoot<T,IndexT>>
    {
    public:

	typedef T value_type;
	typedef OpInterface<T,COpRoot<T,IndexT>> OI;
	
	constexpr COpRoot(const DArrayBase<T>& a, const Sptr<IndexT>& ind);
	constexpr COpRoot(const T* data, const Sptr<IndexT>& ind);

	template <class PosT>
	constexpr decltype(auto) get(const PosT& pos) const;

	template <SizeT I>
	constexpr decltype(auto) rootSteps(const IndexId<I>& id) const;
	
	template <class Xpr>
	constexpr decltype(auto) loop(Xpr&& exp) const;

	const T* data() const;
	
    private:
	const T* mData;
	Sptr<IndexType> mIndex;
    };

    
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
        PointerAccess<T> mDataAcc;
	IndexType mIndex;

    public:
	OperationRoot(MutableArrayBase<T,Ranges...>& ma,
		      const std::shared_ptr<typename Ranges::IndexType>&... indices);

	OperationRoot(MutableArrayBase<T,Ranges...>& ma,
		      const std::tuple<std::shared_ptr<typename Ranges::IndexType>...>& indices);

	OperationRoot(T* data, const IndexType& ind);

        template <class Func, class Access, class OpClass>
        auto asx(const OpClass& in) const;

        template <class Func, class Access, class OpClass>
        auto asxExpr(const OpClass& in) const;
            
        template <class Func, class Access, class OpClass, class Index>
        auto asx(const OpClass& in, const std::shared_ptr<Index>& i) const;
            
        template <class OpClass>
        auto assign(const OpClass& in) const;

	template <class OpClass>
        auto assignExpr(const OpClass& in) const;
	    
        template <class OpClass, class Index>
        auto assign(const OpClass& in, const std::shared_ptr<Index>& i) const;
	
	template <class OpClass>
        auto plus(const OpClass& in) const;

        template <class OpClass, class Index>
        auto plus(const OpClass& in, const std::shared_ptr<Index>& i) const;
            
        template <class OpClass>
        OperationRoot& operator=(const OpClass& in);

        template <class OpClass>
        OperationRoot& operator+=(const OpClass& in);

        OperationRoot& operator=(const OperationRoot& in);
        
	auto par() { return *this; }
        
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
}


#include "type_operations.h"

#endif
