// -*- C++ -*-

#ifndef __multi_array_operation_h__
#define __multi_array_operation_h__

#include <cstdlib>
#include <tuple>

#include "base_def.h"
#include "index_base.h"

namespace MultiArrayTools
{

    template <typename T>
    class MultiArrayOperationBase
    {
    public:

	MultiArrayOperationBase() = default;
	virtual ~MultiArrayOperationBase();

	virtual size_t argNum() const = 0;
	IndefinitIndexBase* index();
	virtual void linkIndicesTo(IndefinitIndexBase* target) const = 0;
	
	virtual T& get() = 0;
	virtual const T& get() const = 0;

    protected:
	IndefinitIndexBase* mIibPtr = nullptr;
    };
    
    template <typename T, class Range>
    class MultiArrayOperationRoot : public MultiArrayOperationBase<T>
    {
    public:

	typedef MultiArrayOperationBase<T> MAOB;
	typedef decltype(MultiArray<T,Range>().begin()) IndexType;
	
	MultiArrayOperationRoot(MultiArrayBase<T,Range>& ma, const Name& nm);
	MultiArrayOperationRoot& operator=(const MultiArrayOperationRoot& in);

	template <class Operation, class... MAOps>
	MultiArrayOperationRoot& operator=(const MultiArrayOperation<T,Operation,MAOps...>& in);
	
	template <class Operation, class... MAOps>
	MultiArrayOperation<T,Operation,MultiArrayOperationRoot<T,Range>, MAOps...>
	operator()(Operation& op, const MAOps&... secs);

	template <class Operation, class... MAOps>
	MultiArrayOperation<T,Operation,MultiArrayOperationRoot<T,Range>, MAOps...>
	operator()(const Operation& op, const MAOps&... secs);
	
	template <class MAOp>
	auto operator+(const MAOp& sec) -> decltype(operator()(std::plus<T>(), sec));

	template <class MAOp>
	auto operator-(const MAOp& sec) -> decltype(operator()(std::minus<T>(), sec));

	template <class MAOp>
	auto operator*(const MAOp& sec) -> decltype(operator()(std::multiplies<T>(), sec));

	template <class MAOp>
	auto operator/(const MAOp& sec) -> decltype(operator()(std::divides<T>(), sec));

	virtual size_t argNum() const override;

	//IndexType& index() ;

	virtual void linkIndicesTo(IndefinitIndexBase* target) const override;

	virtual T& get() override;
	virtual const T& get() const override;
	
    protected:
	
	MultiArrayBase<T,Range>& mArrayRef;
	mutable IndexType mIndex;
	Name mNm;
    };

    template <typename T, class Operation, class... MAOps>
    class MultiArrayOperation : public MultiArrayOperationBase<T>
    {
    public:

	typedef MultiArrayOperationBase<T> MAOB;
	typedef std::tuple<MAOps...> OBT;
	
	MultiArrayOperation(Operation& op, const MAOps&... secs);
	MultiArrayOperation(const Operation& op, const MAOps&... secs);

	template <class Operation2, class... MAOps2>
	MultiArrayOperation<T,Operation2,MultiArrayOperation<T,Operation,MAOps...>,MAOps2...>
	operator()(Operation2& op, const MAOps2&... secs);
	
	template <class Operation2, class... MAOps2>
	MultiArrayOperation<T,Operation2,MultiArrayOperation<T,Operation,MAOps...>,MAOps2...>
	operator()(const Operation2& op, const MAOps2&... secs);
	
	template <class MAOp2>
	auto operator+(const MAOp2& sec) -> decltype(operator()(std::plus<T>(), sec));

	template <class MAOp2>
	auto operator-(const MAOp2& sec) -> decltype(operator()(std::minus<T>(), sec));

	template <class MAOp2>
	auto operator*(const MAOp2& sec) -> decltype(operator()(std::multiplies<T>(), sec));

	template <class MAOp2>
	auto operator/(const MAOp2& sec) -> decltype(operator()(std::divides<T>(), sec));
	
	virtual size_t argNum() const override;
	
	virtual void linkIndicesTo(IndefinitIndexBase* target) const override;

	virtual T& get() override;
	virtual const T& get() const override;

    protected:

 	mutable T mVal;
	Operation mOp;
	OBT mArgs; // include first arg also here !!!
    };



}

#include "multi_array_operation.cc"

#endif
