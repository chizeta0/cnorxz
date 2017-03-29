// -*- C++ -*-

#ifndef __multi_array_operation_h__
#define __multi_array_operation_h__

#include <cstdlib>
#include <tuple>
#include <cmath>

#include "base_def.h"
#include "index_base.h"

namespace MultiArrayTools
{

    template <typename T>
    class MultiArrayOperationBase
    {
    public:

	typedef T value_type;
	
	MultiArrayOperationBase() /*{ CHECK; }*/ = default;
	//MultiArrayOperationBase(const MultiArrayOperationBase& in) = default;
	virtual ~MultiArrayOperationBase();

	virtual size_t argNum() const = 0;
	const IndefinitIndexBase& index() const;
	virtual IndefinitIndexBase* getLinked(const std::string& name) const = 0;
	virtual void linkIndicesTo(IndefinitIndexBase* target) const = 0;
	
	virtual const T& get() const = 0;

	virtual void setInternalLinks() const {}
	
	virtual void freeIndex() const;
	
    protected:
        mutable IndefinitIndexBase* mIibPtr = nullptr;
    };
    
    
    template <typename T>
    class MutableMultiArrayOperationBase : public MultiArrayOperationBase<T>
    {
    public:

	MutableMultiArrayOperationBase() /*{ CHECK; }*/ = default;
	//MutableMultiArrayOperationBase(const MutableMultiArrayOperationBase& in) = default;
	virtual T& get() = 0;
    };
    
    template <typename T, class Range>
    class MultiArrayOperationRoot : public MutableMultiArrayOperationBase<T>
    {
    public:

	typedef MultiArrayOperationBase<T> MAOB;
	typedef typename Range::IndexType IndexType;
	//typedef decltype(MultiArray<T,Range>().begin()) IndexType;

	MultiArrayOperationRoot(MutableMultiArrayBase<T,Range>& ma, const Name& nm);
	MultiArrayOperationRoot(const MultiArrayOperationRoot& in);
	MultiArrayOperationRoot& operator=(const MultiArrayOperationRoot& in);

	MultiArrayOperationRoot& operator=(MultiArrayOperationRoot& in);

	template <class Range2>
	MultiArrayOperationRoot& operator=(MultiArrayOperationRoot<T,Range2>& in);
	
	template <class Range2>
	const MultiArrayOperationRoot& operator=(const MultiArrayOperationRoot<T,Range2>& in);

	template <class Range2>
	MultiArrayOperationRoot& operator=(ConstMultiArrayOperationRoot<T,Range2>& in);
	
	template <class Range2>
	const MultiArrayOperationRoot& operator=(const ConstMultiArrayOperationRoot<T,Range2>& in);

	
	template <class Operation, class... MAOps>
	MultiArrayOperationRoot& operator=(const MultiArrayOperation<T,Operation,MAOps...>& in);

	template <class Operation, class Range2, class... MAOps>
	MultiArrayOperationRoot<T,Range>&
	operator=(const MultiArrayContraction<T,Operation,Range2,MAOps...>& in);

	//template <class Operation, class... MAOps>
	//MultiArrayOperation<T,Operation,MultiArrayOperationRoot<T,Range>, MAOps...>
	//operator()(Operation& op, const MAOps&... secs) const;

	template <class Operation, class... MAOps>
	MultiArrayOperation<T,Operation,MultiArrayOperationRoot<T,Range>, MAOps...>
	operator()(const Operation& op, const MAOps&... secs) const;

	template < class Range2, class ContractOperation, class... MAOps>
	MultiArrayContraction<T,ContractOperation,Range2,MultiArrayOperationRoot<T,Range>, MAOps...>
	contract(const ContractOperation& cop, const std::string& indexName,
		 const MAOps&... mao) const;
	
	template <class Range2, class ContractOperation, class... MAOps>
	MultiArrayContraction<T,ContractOperation,Range2,MultiArrayOperationRoot<T,Range>, MAOps...>
	contract(const ContractOperation& cop, const std::string& indexName,
		 size_t begin,
		 size_t end,
		 const MAOps&... mao) const;

	template<class TotalInRange, class InRange, class OutRange>
	MultiArrayOperationMap<T,InRange,TotalInRange,OutRange,Range>
	map(const IndexMapFunction<InRange,OutRange>& imf);
	
	template <class MAOp>
	auto operator+(const MAOp& sec) -> decltype(operator()(std::plus<T>(), sec));

	template <class MAOp>
	auto operator-(const MAOp& sec) -> decltype(operator()(std::minus<T>(), sec));

	template <class MAOp>
	auto operator*(const MAOp& sec) -> decltype(operator()(std::multiplies<T>(), sec));

	template <class MAOp>
	auto operator/(const MAOp& sec) -> decltype(operator()(std::divides<T>(), sec));
	
	MultiArrayOperationRoot copyThis();
	
	template <class MAOp>
	MultiArrayOperationRoot& operator+=(const MAOp& sec);

	template <class MAOp>
	MultiArrayOperationRoot& operator-=(const MAOp& sec);

	template <class MAOp>
	MultiArrayOperationRoot& operator*=(const MAOp& sec);

	template <class MAOp>
	MultiArrayOperationRoot& operator/=(const MAOp& sec);

	const MultiArrayBase<T,Range>& operator*() const;
	//MultiArrayBase<T,Range>& operator*();
	MultiArrayBase<T,Range> const* operator->() const;
	
	virtual size_t argNum() const override;

	// set index -> implement !!!!!
	MultiArrayOperationRoot<T,Range>& operator[](const IndexType& ind);
	const MultiArrayOperationRoot<T,Range>& operator[](const IndexType& ind) const;

	virtual IndefinitIndexBase* getLinked(const std::string& name) const override;
	virtual void linkIndicesTo(IndefinitIndexBase* target) const override;

	virtual T& get() override;
	virtual const T& get() const override;

	const Name& name() const;

	virtual void freeIndex() const override;

	const MultiArrayBase<T,Range>& getCont() const { return mArrayRef; }
	
	template <typename U, class RangeX>
	friend class MultiArrayOperationRoot;

	template <typename U, class RangeX>
	friend class ConstMultiArrayOperationRoot;
	
    protected:

	void performAssignment(const MultiArrayOperationBase<T>& in);

	MutableMultiArrayBase<T,Range>& mArrayRef;
	mutable IndexType mIndex;
	Name mNm;
    };
    
    template <typename T, class Range>
    class ConstMultiArrayOperationRoot : public MultiArrayOperationBase<T>
    {
    public:

	typedef MultiArrayOperationBase<T> MAOB;
	typedef typename Range::IndexType IndexType;
	//typedef decltype(MultiArray<T,Range>().begin()) IndexType;

	ConstMultiArrayOperationRoot(const MultiArrayBase<T,Range>& ma, const Name& nm);
	ConstMultiArrayOperationRoot(const MultiArrayOperationRoot<T,Range>& in);
	ConstMultiArrayOperationRoot(const ConstMultiArrayOperationRoot& in);
	
	template <class Operation, class... MAOps>
	MultiArrayOperation<T,Operation,ConstMultiArrayOperationRoot<T,Range>, MAOps...>
	operator()(const Operation& op, const MAOps&... secs) const;

	template <class Range2, class ContractOperation, class... MAOps>
	MultiArrayContraction<T,ContractOperation,Range2,ConstMultiArrayOperationRoot<T,Range>, MAOps...>
	contract(const ContractOperation& cop, const std::string& indexName,
		 const MAOps&... mao) const;

	
	template <class Range2, class ContractOperation, class... MAOps>
	MultiArrayContraction<T,ContractOperation,Range2,ConstMultiArrayOperationRoot<T,Range>, MAOps...>
	contract(const ContractOperation& cop, const std::string& indexName,
		 size_t begin,
		 size_t end,
		 const MAOps&... mao) const;
	
	template <class MAOp>
	auto operator+(const MAOp& sec) const -> decltype(operator()(std::plus<T>(), sec));

	template <class MAOp>
	auto operator-(const MAOp& sec) const -> decltype(operator()(std::minus<T>(), sec));

	template <class MAOp>
	auto operator*(const MAOp& sec) const -> decltype(operator()(std::multiplies<T>(), sec));

	template <class MAOp>
	auto operator/(const MAOp& sec) const -> decltype(operator()(std::divides<T>(), sec));
	
	ConstMultiArrayOperationRoot copyThis() const;
	
	const MultiArrayBase<T,Range>& operator*() const;
	//MultiArrayBase<T,Range>& operator*();
	MultiArrayBase<T,Range> const* operator->() const;
	
	virtual size_t argNum() const override;

	// set index -> implement !!!!!
	const ConstMultiArrayOperationRoot<T,Range>& operator[](const IndexType& ind) const;

	virtual IndefinitIndexBase* getLinked(const std::string& name) const override;
	virtual void linkIndicesTo(IndefinitIndexBase* target) const override;

	virtual const T& get() const override;

	const Name& name() const;

	virtual void freeIndex() const override;

	const MultiArrayBase<T,Range>& getCont() const { return mArrayRef; }
	
	template <typename U, class RangeX>
	friend class ConstMultiArrayOperationRoot;
	
	template <typename U, class RangeX>
	friend class MultiArrayOperationRoot;
	
    protected:

	MultiArrayBase<T,Range> const& mArrayRef;
	mutable IndexType mIndex;
	Name mNm;
    };

    template <typename T, class InRange, class TotalInRange, class OutRange, class TotalRange>
    class MultiArrayOperationMap : public MutableMultiArrayOperationBase<T>
    {
    public:
	typedef MultiArrayOperationBase<T> MAOB;
	
	MultiArrayOperationMap(MultiArrayOperationRoot<T,TotalRange>& root,
			       const IndexMapFunction<InRange,OutRange>& mf);

	MultiArrayOperationMap(const MultiArrayOperationMap& in) = default;
	
	MultiArrayOperationMap& operator=(const MultiArrayOperationRoot<T,TotalInRange>& in);
	MultiArrayOperationMap& operator=(const ConstMultiArrayOperationRoot<T,TotalInRange>& in);
	
	virtual size_t argNum() const override;

	virtual IndefinitIndexBase* getLinked(const std::string& name) const override;
	virtual void linkIndicesTo(IndefinitIndexBase* target) const override;

	virtual void setInternalLinks() const override;
	
	virtual const T& get() const override;
	virtual T& get() override;
	// !!!!
    protected:
	IndexMapFunction<InRange,OutRange> mMF;
	MultiArrayOperationRoot<T,TotalRange>& mRoot;
	mutable typename TotalInRange::IndexType mIndex; // Index of incoming range
	Name mNm; // Name of incoming range
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
	operator()(Operation2& op, const MAOps2&... secs) const;
	
	template <class Operation2, class... MAOps2>
	MultiArrayOperation<T,Operation2,MultiArrayOperation<T,Operation,MAOps...>,MAOps2...>
	operator()(const Operation2& op, const MAOps2&... secs) const;

	
	template <class Range2, class ContractOperation, class... MAOps2>
	MultiArrayContraction<T,ContractOperation,Range2,MultiArrayOperation<T,Operation,MAOps...>,MAOps2...>
	contract(const ContractOperation& cop, const std::string& indexName,
		 const MAOps2&... mao) const;

	
	template <class MAOp2>
	auto operator+(const MAOp2& sec) -> decltype(operator()(std::plus<T>(), sec));

	template <class MAOp2>
	auto operator-(const MAOp2& sec) -> decltype(operator()(std::minus<T>(), sec));

	template <class MAOp2>
	auto operator*(const MAOp2& sec) -> decltype(operator()(std::multiplies<T>(), sec));

	template <class MAOp2>
	auto operator/(const MAOp2& sec) -> decltype(operator()(std::divides<T>(), sec));
	
	virtual size_t argNum() const override;
	
	virtual IndefinitIndexBase* getLinked(const std::string& name) const override;
	virtual void linkIndicesTo(IndefinitIndexBase* target) const override;

	virtual const T& get() const override;

    protected:

 	mutable T mVal;
	Operation mOp;
	OBT mArgs; // include first arg also here !!!
    };

    template <typename T, class ContractOperation, class Range, class... MAOps>
    class MultiArrayContraction : public MultiArrayOperationBase<T>
    {
    public:

	typedef MultiArrayOperationBase<T> MAOB;
	typedef std::tuple<MAOps...> OBT;

	MultiArrayContraction(const ContractOperation& cop,
			      const typename Range::IndexType& runIndex,
			      const MAOps&... mao);

	MultiArrayContraction(const ContractOperation& cop,
			      const typename Range::IndexType& runIndex,
			      size_t begin,
			      size_t end,
			      const MAOps&... mao);

	template <class Operation2, class... MAOps2>
	MultiArrayOperation<T,Operation2,MultiArrayContraction<T,ContractOperation,Range,MAOps...>,MAOps2...>
	operator()(Operation2& op, const MAOps2&... secs) const;
	
	template <class Operation2, class... MAOps2>
	MultiArrayOperation<T,Operation2,MultiArrayContraction<T,ContractOperation,Range,MAOps...>,MAOps2...>
	operator()(const Operation2& op, const MAOps2&... secs) const;

	
	template <class Range2, class ContractOperation2, class... MAOps2>
	MultiArrayContraction<T,ContractOperation2,Range2,
			      MultiArrayContraction<T,ContractOperation,Range,MAOps...>,MAOps2...>
	contract(const ContractOperation2& cop, const std::string& indexName,
		 const MAOps2&... mao) const;

	
	template <class MAOp2>
	auto operator+(const MAOp2& sec) -> decltype(operator()(std::plus<T>(), sec));

	template <class MAOp2>
	auto operator-(const MAOp2& sec) -> decltype(operator()(std::minus<T>(), sec));

	template <class MAOp2>
	auto operator*(const MAOp2& sec) -> decltype(operator()(std::multiplies<T>(), sec));

	template <class MAOp2>
	auto operator/(const MAOp2& sec) -> decltype(operator()(std::divides<T>(), sec));

	virtual void setInternalLinks() const override;
	
	virtual size_t argNum() const override;

	virtual IndefinitIndexBase* getLinked(const std::string& name) const override;
	virtual void linkIndicesTo(IndefinitIndexBase* target) const override;

	virtual const T& get() const override;

    protected:

 	mutable T mVal;
	ContractOperation mOp;
	OBT mArgs; // include first arg also here !!!
	typename Range::IndexType mBeginIndex;
	typename Range::IndexType mEndIndex;
        mutable typename Range::IndexType mRunIndex;

    };
}

#include "multi_array_operation.cc"

#endif
