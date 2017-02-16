// -*- C++ -*-

#ifndef __multi_array_operation_h__
#define __multi_array_operation_h__

#include <cstdlib>
#include <tuple>

#include "base_def.h"
#include "index_base.h"

namespace MultiArrayTools
{

    template <typename T, class Range>
    class MultiArrayOperationBase
    {
    public:
	
	MultiArrayOperation(MultiArray<T,Range>& ma, const IndefinitIndexBase& iib);
	
	// execute AnyOperation
	// exception if range types are inconsitent with names
	template <class Range2>
	MultiArrayOperationBase& operator=(const MultiArrayOperationBase<T, Range2>& in);
	
	
	template <class Operation, class... Ranges>
	MultiArrayOperation<Operation> operator()(Operation& op, MultiArrayOperationBase<T,Ranges>&... secs);
	
	template <class Range2>
	MultiArrayOperation<std::plus<T>,Range2> operator+(MultiArrayOperationBase<T,Range2>& sec);

	template <class Range2>
	MultiArrayOperation<std::minus<T>,Range2> operator-(MultiArrayOperationBase<T,Range2>& sec);

	template <class Range2>
	MultiArrayOperation<std::multiplies<T>,Range2> operator*(MultiArrayOperationBase<T,Range2>& sec);

	template <class Range2>
	MultiArrayOperation<std::divides<T>,Range2> operator/(MultiArrayOperationBase<T,Range2>& sec);

	virtual size_t argNum() const;

	IndefinitIndexBase* index();

	virtual void linkIndicesTo(IndefinitIndexBase* target);

	virtual T& get();
	virtual const T& get() const;
	
    protected:

	MultiArray<T,Range>& mArrayRef;
	IndefinitIndexBase* mIibPtr;
	
    };

    template <typename T, class Range, class Operation, class... Ranges>
    class MultiArrayOperation : public MultiArrayOperationBase<T,Range>
    {
    public:

	MultiArrayOperation(Operation& op, MultiArrayOperationBase<T,Ranges>&... secs);
	virtual size_t argNum() const override;
	
	virtual void linkIndicesTo(IndefinitIndexBase* target) override;

	virtual T& get() override;
	virtual const T& get() const override;
	
    protected:

	T mVal;
	Operation mOp;
	std::tuple<MultiArrayOperationBase<T,Ranges>... > mSecs;
    };



}

#include "multi_array_operation.h"

#endif
