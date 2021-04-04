
#ifndef __dynamic_operation_h__
#define __dynamic_operation_h__

#include "base_def.h"
#include "multi_array_operation.h"

namespace MultiArrayTools
{

    template <typename T>
    class DynamicOperationBase : public OperationTemplate<T,DynamicOperationBase<T>>
    {
    public:
	typedef T value_type;
	typedef OperationBase<T,DynamicOperationBase<T>> OT;
	
	static constexpr size_t SIZE = 1;
	static constexpr bool CONT = true;
	static constexpr bool VABLE = false;
	
	DynamicOperationBase() = default;
	DynamicOperationBase(const DynamicOperationBase& in) = default;
	DynamicOperationBase(DynamicOperationBase&& in) = default;
	DynamicOperationBase& operator=(const DynamicOperationBase& in) = default;
	DynamicOperationBase& operator=(DynamicOperationBase&& in) = default;

	virtual T get(const DExtT& pos) const = 0;
	virtual DynamicOperationBase& set(const DExtT& pos) = 0;
	virtual DExtT rootSteps(std::intptr_t iPtrNum = 0) const = 0;
	virtual DynamicExpression loop(const DynamicExpression& exp) const = 0;
	virtual const T* data() const = 0;
        virtual std::shared_ptr<DynamicOperationBase<T>> deepCopy() const = 0;
	
    };

    template <typename T, class Operation>
    class DynamicOperation : public DynamicOperationBase<T>
    {
    private:
	Operation mOp;
    public:
        typedef decltype(mOp.rootSteps()) ET;
	//typedef decltype(std::declval<Operation>().rootSteps()) ET;
	
	DynamicOperation() = default;
	DynamicOperation(const DynamicOperation& in) = default;
	DynamicOperation(DynamicOperation&& in) = default;
	DynamicOperation& operator=(const DynamicOperation& in) = default;
	DynamicOperation& operator=(DynamicOperation&& in) = default;

	DynamicOperation(const Operation& op) : mOp(op) {}

	virtual T get(const DExtT& pos) const override final;
	virtual DynamicOperationBase<T>& set(const DExtT& pos) override final;
	virtual DExtT rootSteps(std::intptr_t iPtrNum = 0) const override final;
	virtual DynamicExpression loop(const DynamicExpression& exp) const override final;
	virtual const T* data() const override final;
        virtual std::shared_ptr<DynamicOperationBase<T>> deepCopy() const override final;
    };

    template <class Op>
    struct OpH
    {
        std::shared_ptr<Op> mOp;
	OpH(const Op& op) : mOp(std::make_shared<Op>(op)) {}

	// overload all operations here ...
    };
    
    template <typename T, class Operation, class... Ranges>
    class DynamicOuterOp : public DynamicOperationBase<OpH<OperationRoot<T,Ranges...>>>
    {
    private:
	size_t mThreadId;
	//std::shared_ptr<DynamicOperationBase<OpH<OperationRoot<T,Ranges...>>>> mDyn;
	Operation mOp;
        //OperationRoot<T,Ranges...> mProto;
	std::tuple<std::shared_ptr<typename Ranges::IndexType>...> mIndices;
	std::shared_ptr<MultiArray<T,Ranges...>> mMa;
        OpH<OperationRoot<T,Ranges...>> mProto;

	
	typedef ILoop<std::tuple<OperationRoot<T,Ranges...>,Operation>,
		      std::tuple<std::shared_ptr<typename Ranges::IndexType>...>,
		      std::tuple<std::shared_ptr<MultiArray<T,Ranges...>>>,
            std::tuple<decltype(mProto.mOp->assign( mOp, mkMIndex(std::shared_ptr<typename Ranges::IndexType>()...) ))>> LoopT;
	
	
	
	mutable LoopT mL;
	
    public:
        typedef decltype(mL.rootSteps()) ET;
	//typedef decltype(std::declval<Operation>().rootSteps()) ET;
	
	DynamicOuterOp() : mThreadId(omp_get_thread_num()) {}
	DynamicOuterOp(const DynamicOuterOp& in);
	DynamicOuterOp(DynamicOuterOp&& in);
	DynamicOuterOp& operator=(const DynamicOuterOp& in);
	DynamicOuterOp& operator=(DynamicOuterOp&& in);

	DynamicOuterOp(const Operation& op, const std::shared_ptr<typename Ranges::IndexType>&... inds);
	/*
	DynamicOuterOp(const std::shared_ptr<DynamicOperationBase<OpH<OperationRoot<typename Operatrion::value_type,Ranges...>>>>& dyn,
		       const Operation& op, const std::shared_ptr<Indices>&... inds	);
	*/
	virtual OpH<OperationRoot<T,Ranges...>> get(const DExtT& pos) const override final;
	virtual DynamicOperationBase<OpH<OperationRoot<T,Ranges...>>>& set(const DExtT& pos) override final;
	virtual DExtT rootSteps(std::intptr_t iPtrNum = 0) const override final;
	virtual DynamicExpression loop(const DynamicExpression& exp) const override final;
	virtual const OpH<OperationRoot<T,Ranges...>>* data() const override final;
        virtual std::shared_ptr<DynamicOperationBase<OpH<OperationRoot<T,Ranges...>>>> deepCopy() const override final;
    };

    
    template <typename T>
    class DynamicO : public OperationTemplate<T,DynamicO<T>>
    {
    private:
	// NOT THREAD SAFE!!!
	std::shared_ptr<DynamicOperationBase<T>> mOp;
    public:
	typedef T value_type;
	typedef OperationBase<T,DynamicO<T>> OT;
	
	static constexpr size_t SIZE = 1;
	static constexpr bool CONT = true;
	static constexpr bool VABLE = false;
	
	DynamicO() = default;
	DynamicO(const DynamicO& in) : mOp(in.mOp ? in.mOp->deepCopy() : nullptr) {}
	DynamicO(DynamicO&& in) : mOp(in.mOp ? in.mOp->deepCopy() : nullptr) {}
	DynamicO& operator=(const DynamicO& in)
	{ mOp = in.mOp ? in.mOp->deepCopy() : nullptr; return *this; }
	DynamicO& operator=(DynamicO&& in)
	{ mOp = in.mOp ? in.mOp->deepCopy() : nullptr; return *this; }

	bool init() const { return mOp != nullptr; }
	
	template <class Op>
	DynamicO(const Op& op) : mOp(std::make_shared<DynamicOperation<T,Op>>(op)) {}

	DynamicO(const std::shared_ptr<DynamicOperationBase<T>>& op) :
	    mOp(op) {}
	
        template <class X>
	inline T get(const DExtTX<X>& pos) const { return mOp->get(pos.reduce()); }

	template <typename V,class X>
	inline auto vget(const DExtTX<X>& pos) const
	{ return mOp->template vget<V>(pos.reduce()); }
	
        template <class X>
	inline DynamicO& set(const DExtTX<X>& pos) { mOp->set(pos.reduce()); return *this; }
	inline DExtT rootSteps(std::intptr_t iPtrNum = 0) const { return mOp->rootSteps(iPtrNum); }
	inline DynamicExpression loop(const DynamicExpression& exp) const { return mOp->loop(exp); }
	inline const T* data() const { return mOp->data(); }
    };
    /*
    template <class Op1>
    class TwoOp : public OperationTemplate<typename Op2::value_type,TwoOp<Op1>>
    {
    private:

	Op1 mOp1;
	typename Op1::value_type mOp2; // mOp1.data()->mOp

    public:

	typedef typename Op2::value_type value_type;
	typedef value_type T;
	
	TwoOp(const Op1& op1);

	template <class ET>
	inline T get(const ET& pos) const;
    };
    */
    template <class Operation, class... Indices>
    auto mkDynOutOp(const Operation& op, const std::shared_ptr<Indices>&... inds)
    {
	return DynamicO<OpH<OperationRoot<typename Operation::value_type,
					  typename Indices::RangeType...>>>
	    (DynamicOuterOp<typename Operation::value_type,Operation,
	     typename Indices::RangeType...>(op, inds...));
    }
    /*
    template <class Operation, class... Indices>
    auto mkDynOutOp(const std::shared_ptr<DynamicOperationBase<OpH<OperationRoot<typename Operatrion::value_type,Ranges...>>>>& dyn,
		    const Operation& op, const std::shared_ptr<Indices>&... inds)
    {
	return DynamicO<OpH<OperationRoot<typename Operation::value_type,
					  typename Indices::RangeType...>>>
	    (DynamicOuterOp<typename Operation::value_type,Operation,
	     typename Indices::RangeType...>(dyn, op, inds...));
    }
    */
    // Build plan
    /*
    template <class Operation>
    class OperationBuilder
    {

    };
    */
} // namespace MultiArrayTools

#endif
