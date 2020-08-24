
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
	
	DynamicOperationBase() = default;
	DynamicOperationBase(const DynamicOperationBase& in) = default;
	DynamicOperationBase(DynamicOperationBase&& in) = default;
	DynamicOperationBase& operator=(const DynamicOperationBase& in) = default;
	DynamicOperationBase& operator=(DynamicOperationBase&& in) = default;

	virtual const T& get(const DExtT& pos) const = 0;
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

	virtual const T& get(const DExtT& pos) const override final;
	virtual DynamicOperationBase<T>& set(const DExtT& pos) override final;
	virtual DExtT rootSteps(std::intptr_t iPtrNum = 0) const override final;
	virtual DynamicExpression loop(const DynamicExpression& exp) const override final;
	virtual const T* data() const override final;
        virtual std::shared_ptr<DynamicOperationBase<T>> deepCopy() const override final;
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
	
	DynamicO() = default;
	DynamicO(const DynamicO& in) : mOp(in.mOp->deepCopy()) {}
	DynamicO(DynamicO&& in) : mOp(in.mOp->deepCopy()) {}
	DynamicO& operator=(const DynamicO& in) { mOp = in.mOp->deepCopy(); }
	DynamicO& operator=(DynamicO&& in) { mOp = in.mOp->deepCopy(); }

	template <class Op>
	DynamicO(const Op& op) : mOp(std::make_shared<DynamicOperation<T,Op>>(op)) {}

        template <class X>
	inline const T& get(const DExtTX<X>& pos) const { return mOp->get(pos.reduce()); }
	inline DynamicO& set(const DExtT& pos) { return mOp->set(pos); }
	inline DExtT rootSteps(std::intptr_t iPtrNum = 0) const { return mOp->rootSteps(iPtrNum); }
	inline DynamicExpression loop(const DynamicExpression& exp) const { return mOp->loop(exp); }
	inline const T* data() const { return mOp->data(); }
    };
    
} // namespace MultiArrayTools

#endif
