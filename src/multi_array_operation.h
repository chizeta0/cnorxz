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

	typedef decltype(MultiArray<T,Range>().begin()) IndexType;
	
	MultiArrayOperationBase(MultiArray<T,Range>& ma, const Name& nm);
	MultiArrayOperationBase& operator=(const MultiArrayOperationBase& in);
	//MultiArrayOperationBase(const MultiArrayOperationBase& in) = default;
	
	virtual ~MultiArrayOperationBase();
	
	// execute AnyOperation
	// exception if range types are inconsitent with names
	template <class Range2>
	MultiArrayOperationBase& operator=(const MultiArrayOperationBase<T, Range2>& in);
	
	
	template <class Operation, class... Ranges>
	MultiArrayOperation<T,Range,Operation,Ranges...>
	operator()(Operation& op, const MultiArrayOperationBase<T,Ranges>&... secs);

	template <class Operation, class... Ranges>
	MultiArrayOperation<T,Range,Operation,Ranges...>
	operator()(const Operation& op, const MultiArrayOperationBase<T,Ranges>&... secs);
	
	template <class Range2>
	MultiArrayOperation<T,Range,std::plus<T>,Range2> operator+(const MultiArrayOperationBase<T,Range2>& sec);

	template <class Range2>
	MultiArrayOperation<T,Range,std::minus<T>,Range2> operator-(const MultiArrayOperationBase<T,Range2>& sec);

	template <class Range2>
	MultiArrayOperation<T,Range,std::multiplies<T>,Range2> operator*(const MultiArrayOperationBase<T,Range2>& sec);

	template <class Range2>
	MultiArrayOperation<T,Range,std::divides<T>,Range2> operator/(const MultiArrayOperationBase<T,Range2>& sec);

	virtual size_t argNum() const;

	//IndexType& index() ;
	IndefinitIndexBase* index();

	virtual void linkIndicesTo(IndefinitIndexBase* target) const;

	virtual T& get();
	virtual const T& get() const;

    protected:

	MultiArray<T,Range>& mArrayRef;
	//mutable IndexType mIndex;
	IndefinitIndexBase* mIibPtr = nullptr;
	Name mNm;
    };

    template <typename T, class Range, class Operation, class... Ranges>
    class MultiArrayOperation : public MultiArrayOperationBase<T,Range>
    {
    public:

	typedef MultiArrayOperationBase<T,Range> OB;
	typedef std::tuple<MultiArrayOperationBase<T,Ranges>... > OBT;
	
	MultiArrayOperation(MultiArray<T,Range>& ma, const Name& nm,
			    Operation& op, const MultiArrayOperationBase<T,Ranges>&... secs);

	MultiArrayOperation(MultiArray<T,Range>& ma, const Name& nm,
			    const Operation& op, const MultiArrayOperationBase<T,Ranges>&... secs);

	virtual size_t argNum() const override;
	
	virtual void linkIndicesTo(IndefinitIndexBase* target) const override;

	virtual T& get() override;
	virtual const T& get() const override;
	
    protected:

 	mutable T mVal;
	Operation mOp;
	OBT mSecs;
    };



}

#include "multi_array_operation.cc"

#endif
