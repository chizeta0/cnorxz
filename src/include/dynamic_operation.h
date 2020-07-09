
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

	virtual const T& get(const DExt& pos) const = 0;
	virtual DynamicOperationBase& set(const DExt& pos) = 0;
	virtual DExt rootSteps(std::intptr_t iPtrNum = 0) const = 0;
	virtual DynamicExpression loop(const DynamicExpression& exp) const = 0;
	virtual const T* data() const = 0;
	
    };

    template <typename T, class Operation>
    class DynamicOperation : public DynamicOperationBase<T>
    {
    private:
	Operation mOp;
    public:
	typedef decltype(std::declval<Operation>().rootSteps()) ET;
	
	DynamicOperation() = default;
	DynamicOperation(const DynamicOperation& in) = default;
	DynamicOperation(DynamicOperation&& in) = default;
	DynamicOperation& operator=(const DynamicOperation& in) = default;
	DynamicOperation& operator=(DynamicOperation&& in) = default;

	DynamicOperation(const Operation& op) : mOp(op) {}

	virtual const T& get(const DExt& pos) const override final;
	virtual DynamicOperationBase& set(const DExt& pos) override final;
	virtual DExt rootSteps(std::intptr_t iPtrNum = 0) const override final;
	virtual DynamicExpression loop(const DynamicExpression& exp) const override final;
	virtual const T* data() const override final;
    };
    
} // namespace MultiArrayTools

#endif
