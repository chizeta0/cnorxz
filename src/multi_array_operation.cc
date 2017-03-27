// -*- C++ -*-

#include "multi_array_operation.h"

namespace MultiArrayTools
{

    /*********************************
     *   MultiArrayOperationRoot     *
     *********************************/
   
    template <typename T>
    MultiArrayOperationBase<T>::~MultiArrayOperationBase()
    {
	//delete mIibPtr;
    }

    template <typename T>
    const IndefinitIndexBase& MultiArrayOperationBase<T>::index() const
    {
	return *mIibPtr;
    }
    
    template <typename T>
    void MultiArrayOperationBase<T>::freeIndex() const
    {
	mIibPtr->freeLinked();
    }
    
    /*********************************
     *   MultiArrayOperationBase     *
     *********************************/

    template <typename T, class Range>
    void MultiArrayOperationRoot<T,Range>::performAssignment(const MultiArrayOperationBase<T>& in)
    {
	//#error "WRITE MAOR INTRINSIC CONTRACT FUNCTION"
	//CHECK;
	in.linkIndicesTo(MAOB::mIibPtr);
	//CHECK;
	IndexType& iref = dynamic_cast<IndexType&>(*MAOB::mIibPtr);
	//CHECK;
	const size_t endPos = mArrayRef.endIndex().pos();
	std::cout << "assignment: " << endPos << " elements" << std::endl;
	// assignment loop
	for(iref = mArrayRef.beginIndex().pos(); iref != mArrayRef.endIndex(); ++iref){
	    //std::cout << iref.pos() << '\r' << std::flush;
	    get() = in.get();
	    //assert(not std::isnan( get() ));
	}
	//CHECK;
	MAOB::mIibPtr->freeLinked();
    }

    template <typename T, class Range>
    MultiArrayOperationRoot<T,Range>::
    MultiArrayOperationRoot(MutableMultiArrayBase<T,Range>& ma,
			    const Name& nm) :
	MutableMultiArrayOperationBase<T>(),
	mArrayRef(ma),
	mIndex(mArrayRef.beginIndex()),
	mNm(nm)
    {
	MAOB::mIibPtr = &mIndex;
	MAOB::mIibPtr->name(nm);
	//mIndex.name(nm);
    }

    template <typename T, class Range>
    MultiArrayOperationRoot<T,Range>::
    MultiArrayOperationRoot(const MultiArrayOperationRoot& in) :
	MutableMultiArrayOperationBase<T>(),
	mArrayRef(in.mArrayRef),
	mIndex(mArrayRef.beginIndex()),
	mNm(in.mNm)
    {
	MAOB::mIibPtr = &mIndex;
	MAOB::mIibPtr->name(mNm);
	//mIndex.name(nm);
    }
    
    template <typename T, class Range>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator=(const MultiArrayOperationRoot<T,Range>& in)
    {
	performAssignment(in);
	freeIndex();
	return *this;
    }

    template <typename T, class Range>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator=(MultiArrayOperationRoot<T,Range>& in)
    {
	performAssignment(in);
	freeIndex();
	return *this;
    }

    template <typename T, class Range>
    template <class Range2>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator=(MultiArrayOperationRoot<T,Range2>& in)
    {
	performAssignment(in);
	freeIndex();
	return *this;
    }

    
    template <typename T, class Range>
    template <class Range2>
    const MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator=(const MultiArrayOperationRoot<T,Range2>& in)
    {
	performAssignment(in);
	freeIndex();
	return *this;
    }
    
    template <typename T, class Range>
    template <class Operation, class... MAOps>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator=(const MultiArrayOperation<T,Operation,MAOps...>& in)
    {
   	performAssignment(in);
	freeIndex();
	return *this;
    }
    
    template <typename T, class Range>
    template <class Operation, class Range2, class... MAOps>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator=(const MultiArrayContraction<T,Operation,Range2,MAOps...>& in)
    {
   	performAssignment(in);
	freeIndex();
	return *this;
    }
    
    template <typename T, class Range>
    template <class Operation, class... MAOps>
    MultiArrayOperation<T,Operation,MultiArrayOperationRoot<T,Range>, MAOps...>
    MultiArrayOperationRoot<T,Range>::operator()(const Operation& op, const MAOps&... secs) const
    {
	//CHECK;
	return MultiArrayOperation<T,Operation,MultiArrayOperationRoot<T,Range>, MAOps...>(op, *this, secs...);
    }

    template <typename T, class Range>
    template <class Range2, class ContractOperation, class... MAOps>
    MultiArrayContraction<T,ContractOperation,Range2,MultiArrayOperationRoot<T,Range>,MAOps...>
    MultiArrayOperationRoot<T,Range>::contract(const ContractOperation& cop,
					       const std::string& indexName,
					       const MAOps&... mao) const
    {
	typename Range2::IndexType* ind = dynamic_cast<typename Range2::IndexType*>( mIndex.getLinked(indexName) );
	return MultiArrayContraction<T,ContractOperation,Range2,
				     MultiArrayOperationRoot<T,Range>,
				     MAOps...>(cop, *ind, *this, mao...);
    }

    template <typename T, class Range>
    template <class Range2, class ContractOperation, class... MAOps>
    MultiArrayContraction<T,ContractOperation,Range2,MultiArrayOperationRoot<T,Range>,MAOps...>
    MultiArrayOperationRoot<T,Range>::contract(const ContractOperation& cop,
					       const std::string& indexName,
					       size_t begin,
					       size_t end,
					       const MAOps&... mao) const
    {
	typename Range2::IndexType* ind = dynamic_cast<typename Range2::IndexType*>( mIndex.getLinked(indexName) );
	return MultiArrayContraction<T,ContractOperation,Range2,
				     MultiArrayOperationRoot<T,Range>,
				     MAOps...>(cop, *ind, begin, end, *this, mao...);
    }

    
    template <typename T, class Range>
    template <class MAOp>
    auto MultiArrayOperationRoot<T,Range>::operator+(const MAOp& sec)
	-> decltype(operator()(std::plus<T>(), sec))
    {
	return operator()(std::plus<T>(), sec);
    }

    template <typename T, class Range>
    template <class MAOp>
    auto MultiArrayOperationRoot<T,Range>::operator-(const MAOp& sec)
	-> decltype(operator()(std::minus<T>(), sec))
    {
	return operator()(std::minus<T>(), sec);
    }

    template <typename T, class Range>
    template <class MAOp>
    auto MultiArrayOperationRoot<T,Range>::operator*(const MAOp& sec)
	-> decltype(operator()(std::multiplies<T>(), sec))
    {
	return operator()(std::multiplies<T>(), sec);
    }

    template <typename T, class Range>
    template <class MAOp>
    auto MultiArrayOperationRoot<T,Range>::operator/(const MAOp& sec)
	-> decltype(operator()(std::divides<T>(), sec))
    {
	//CHECK;
	return operator()(std::divides<T>(), sec);
    }

    template <typename T, class Range>
    MultiArrayOperationRoot<T,Range> MultiArrayOperationRoot<T,Range>::copyThis()
    {
	//CHECK;
	return MultiArrayOperationRoot<T,Range>(mArrayRef, mNm);
    }
    
    template <typename T, class Range>
    template <class MAOp>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator+=(const MAOp& sec)
    {
	return (*this) = copyThis() + sec;
    }

    template <typename T, class Range>
    template <class MAOp>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator-=(const MAOp& sec)
    {
	return (*this) = copyThis() - sec;
    }

    template <typename T, class Range>
    template <class MAOp>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator*=(const MAOp& sec)
    {
	return (*this) = copyThis() * sec;
    }

    template <typename T, class Range>
    template <class MAOp>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator/=(const MAOp& sec)
    {
	//CHECK;
	return (*this) = copyThis() / sec;
    }

    template <typename T, class Range>
    const MultiArrayBase<T,Range>& MultiArrayOperationRoot<T,Range>::operator*() const
    {
	return mArrayRef;
    }

    template <typename T, class Range>
    MultiArrayBase<T,Range> const* MultiArrayOperationRoot<T,Range>::operator->() const
    {
	return &mArrayRef;
    }
    
    template <typename T, class Range>
    size_t MultiArrayOperationRoot<T,Range>::argNum() const
    {
	return 1;
    }

    template <typename T, class Range>
    IndefinitIndexBase* MultiArrayOperationRoot<T,Range>::getLinked(const std::string& name) const
    {
	return mIndex.getLinked(name);
    }
    
    template <typename T, class Range>
    void MultiArrayOperationRoot<T,Range>::linkIndicesTo(IndefinitIndexBase* target) const
    {
	MAOB::mIibPtr = &mIndex;
	MAOB::mIibPtr->linkTo(target);
    }

    template <typename T, class Range>
    T& MultiArrayOperationRoot<T,Range>::get()
    {
	//CHECK;
	//return mArrayRef[mIndex];
	assert(MAOB::mIibPtr == &mIndex);
	//VCHECK(mArrayRef[*dynamic_cast<IndexType*>(MAOB::mIibPtr)]);
	return mArrayRef[*dynamic_cast<IndexType*>(MAOB::mIibPtr)];
    }

    template <typename T, class Range>
    const T& MultiArrayOperationRoot<T,Range>::get() const
    {
	//CHECK;
	//return mArrayRef[mIndex];
	assert(MAOB::mIibPtr == &mIndex);
	//VCHECK(mArrayRef[*dynamic_cast<IndexType*>(MAOB::mIibPtr)]);
	return mArrayRef[*dynamic_cast<IndexType*>(MAOB::mIibPtr)];
    }

    template <typename T, class Range>
    const Name& MultiArrayOperationRoot<T,Range>::name() const
    {
	return mNm;
    }

    template <typename T, class Range>
    MultiArrayOperationRoot<T,Range>& MultiArrayOperationRoot<T,Range>::operator[](const IndexType& ind)
    {
	mIndex.copyPos(ind);
	return *this;
    }

    template <typename T, class Range>
    const MultiArrayOperationRoot<T,Range>& MultiArrayOperationRoot<T,Range>::operator[](const IndexType& ind) const
    {
	mIndex.copyPos(ind);
	return *this;
    }

    template <typename T, class Range>
    void MultiArrayOperationRoot<T,Range>::freeIndex() const
    {
	MAOB::mIibPtr->freeLinked();
	mIndex = mArrayRef.beginIndex();
	MAOB::mIibPtr = &mIndex;
	MAOB::mIibPtr->name(mNm);
    }

    /**************************************
     *   ConstMultiArrayOperationBase     *
     **************************************/

   
    template <typename T, class Range>
    ConstMultiArrayOperationRoot<T,Range>::
    ConstMultiArrayOperationRoot(const MultiArrayBase<T,Range>& ma,
				 const Name& nm) :
	MultiArrayOperationBase<T>(),
	mArrayRef(ma),
	mIndex(mArrayRef.beginIndex()),
	mNm(nm)
    {
	MAOB::mIibPtr = &mIndex;
	MAOB::mIibPtr->name(nm);
	//mIndex.name(nm);
    }

    template <typename T, class Range>
    ConstMultiArrayOperationRoot<T,Range>::
    ConstMultiArrayOperationRoot(const MultiArrayOperationRoot<T,Range>& in) :
	MultiArrayOperationBase<T>(),
	mArrayRef(in.getCont()),
	mIndex(mArrayRef.beginIndex()),
	mNm(in.name())
    {
	MAOB::mIibPtr = &mIndex;
	MAOB::mIibPtr->name(mNm);
	//mIndex.name(nm);
    }

    template <typename T, class Range>
    ConstMultiArrayOperationRoot<T,Range>::
    ConstMultiArrayOperationRoot(const ConstMultiArrayOperationRoot& in) :
	MultiArrayOperationBase<T>(),
	mArrayRef(in.mArrayRef),
	mIndex(mArrayRef.beginIndex()),
	mNm(in.mNm)
    {
	MAOB::mIibPtr = &mIndex;
	MAOB::mIibPtr->name(mNm);
	//mIndex.name(nm);
    }
    
    template <typename T, class Range>
    template <class Operation, class... MAOps>
    MultiArrayOperation<T,Operation,ConstMultiArrayOperationRoot<T,Range>, MAOps...>
    ConstMultiArrayOperationRoot<T,Range>::operator()(const Operation& op, const MAOps&... secs) const
    {
	//CHECK;
	return MultiArrayOperation<T,Operation,ConstMultiArrayOperationRoot<T,Range>, MAOps...>(op, *this, secs...);
    }

    template <typename T, class Range>
    template <class Range2, class ContractOperation, class... MAOps>
    MultiArrayContraction<T,ContractOperation,Range2,ConstMultiArrayOperationRoot<T,Range>,MAOps...>
    ConstMultiArrayOperationRoot<T,Range>::contract(const ContractOperation& cop,
						    const std::string& indexName,
						    const MAOps&... mao) const
    {
	typename Range2::IndexType* ind = dynamic_cast<typename Range2::IndexType*>( mIndex.getLinked(indexName) );
	return MultiArrayContraction<T,ContractOperation,Range2,
				     ConstMultiArrayOperationRoot<T,Range>,
				     MAOps...>(cop, *ind, *this, mao...);
    }

    template <typename T, class Range>
    template <class Range2, class ContractOperation, class... MAOps>
    MultiArrayContraction<T,ContractOperation,Range2,ConstMultiArrayOperationRoot<T,Range>,MAOps...>
    ConstMultiArrayOperationRoot<T,Range>::contract(const ContractOperation& cop,
						    const std::string& indexName,
						    size_t begin,
						    size_t end,
						    const MAOps&... mao) const
    {
	typename Range2::IndexType* ind = dynamic_cast<typename Range2::IndexType*>( mIndex.getLinked(indexName) );
	return MultiArrayContraction<T,ContractOperation,Range2,
				     ConstMultiArrayOperationRoot<T,Range>,
				     MAOps...>(cop, *ind, begin, end, *this, mao...);
    }


    template <typename T, class Range>
    template <class MAOp>
    auto ConstMultiArrayOperationRoot<T,Range>::operator+(const MAOp& sec) const
	-> decltype(operator()(std::plus<T>(), sec))
    {
	return operator()(std::plus<T>(), sec);
    }

    template <typename T, class Range>
    template <class MAOp>
    auto ConstMultiArrayOperationRoot<T,Range>::operator-(const MAOp& sec) const
	-> decltype(operator()(std::minus<T>(), sec))
    {
	return operator()(std::minus<T>(), sec);
    }

    template <typename T, class Range>
    template <class MAOp>
    auto ConstMultiArrayOperationRoot<T,Range>::operator*(const MAOp& sec) const
	-> decltype(operator()(std::multiplies<T>(), sec))
    {
	return operator()(std::multiplies<T>(), sec);
    }

    template <typename T, class Range>
    template <class MAOp>
    auto ConstMultiArrayOperationRoot<T,Range>::operator/(const MAOp& sec) const
	-> decltype(operator()(std::divides<T>(), sec))
    {
	//CHECK;
	return operator()(std::divides<T>(), sec);
    }

    template <typename T, class Range>
    ConstMultiArrayOperationRoot<T,Range> ConstMultiArrayOperationRoot<T,Range>::copyThis() const
    {
	//CHECK;
	return ConstMultiArrayOperationRoot<T,Range>(mArrayRef, mNm);
    }
    
    template <typename T, class Range>
    const MultiArrayBase<T,Range>& ConstMultiArrayOperationRoot<T,Range>::operator*() const
    {
	return mArrayRef;
    }

    template <typename T, class Range>
    MultiArrayBase<T,Range> const* ConstMultiArrayOperationRoot<T,Range>::operator->() const
    {
	return &mArrayRef;
    }
    
    template <typename T, class Range>
    size_t ConstMultiArrayOperationRoot<T,Range>::argNum() const
    {
	return 1;
    }

    template <typename T, class Range>
    IndefinitIndexBase* ConstMultiArrayOperationRoot<T,Range>::getLinked(const std::string& name) const
    {
	return mIndex.getLinked(name);
    }
    
    template <typename T, class Range>
    void ConstMultiArrayOperationRoot<T,Range>::linkIndicesTo(IndefinitIndexBase* target) const
    {
	MAOB::mIibPtr = &mIndex;
	MAOB::mIibPtr->linkTo(target);
    }

    template <typename T, class Range>
    const T& ConstMultiArrayOperationRoot<T,Range>::get() const
    {
	//CHECK;
	assert(MAOB::mIibPtr == &mIndex);
	return mArrayRef[*dynamic_cast<IndexType*>(MAOB::mIibPtr)];
    }

    template <typename T, class Range>
    const Name& ConstMultiArrayOperationRoot<T,Range>::name() const
    {
	return mNm;
    }

    template <typename T, class Range>
    const ConstMultiArrayOperationRoot<T,Range>& ConstMultiArrayOperationRoot<T,Range>::operator[](const IndexType& ind) const
    {
	mIndex.copyPos(ind);
	return *this;
    }

    template <typename T, class Range>
    void ConstMultiArrayOperationRoot<T,Range>::freeIndex() const
    {
	MAOB::mIibPtr->freeLinked();
	mIndex = mArrayRef.beginIndex();
	MAOB::mIibPtr = &mIndex;
	MAOB::mIibPtr->name(mNm);
    }

    /********************************
     *   MultiArrayOperationMap     *
     ********************************/

    template <typename T, class MapFunction, class InRange, class OutRange>
    MultiArrayOperationMap<T,MapFunction,InRange,OutRange>::
    MultiArrayOperationMap(MultiArrayOperationRoot<T,OutRange>& root, const MapFunction mf) :
	MultiArrayOperationBase<T>(),
	mMF(mf),
	mRoot(root)
	//mIndex(mArrayRef.beginIndex()),
	//mNm(nm)
    {
	MAOB::mIibPtr = &mIndex;
	//MAOB::mIibPtr->name(nm);
	//mIndex.name(nm);
    }
	
    template <typename T, class MapFunction, class InRange, class OutRange>
    MultiArrayOperationMap& MultiArrayOperationMap<T,MapFunction,InRange,OutRange>::
    operator=(const ConstMultiArrayOperationRoot<T,InRange>& in)
    {
	mIndex = dynamic_cast<typename InRange::IndexType&>( in.getIndex() );
	MAOB::mIibPtr = &mIndex;
	mNm = in.name();
	mIndex.name(mNm); // to be sure...
	mIndex.setPos( mIndex.max() );
	typename OutRange::IndexType endIndex = mIndex;

	// Implement Map Functions !!!!
	mRoot.linkIndicesTo( &mMF.index() );
	mMF.linkIndicesTo( &mIndex );

	MultiArray<size_t,OutRange> cnt(mRoot->range());
	auto cnto = cnt(mRoot.name(), true);
	cnto.linkIndicesTo( &mMF.index() );
	
	for(mIndex.setPos(0), mMF.eval(); mIndex != endIndex; ++mIndex, mMF.eval()){
	    get() += in.get();
	    ++cnto.get();
	}
	// CHECK whether T / size_t mixture works!!
	mRoot /= cnto;
    }

    template <typename T, class MapFunction, class InRange, class OutRange>
    size_t MultiArrayOperationMap<T,MapFunction,InRange,OutRange>::argNum() const
    {
	return 1;
    }

    template <typename T, class MapFunction, class InRange, class OutRange>
    IndefinitIndexBase* MultiArrayOperationMap<T,MapFunction,InRange,OutRange>::
    getLinked(const std::string& name) const
    {
	return mRoot.getLinked(name);
    }

    template <typename T, class MapFunction, class InRange, class OutRange>
    void MultiArrayOperationMap<T,MapFunction,InRange,OutRange>::linkIndicesTo(IndefinitIndexBase* target) const
    {
	mRoot.linkIndicesTo(target);
    }

    template <typename T, class MapFunction, class InRange, class OutRange>
    void MultiArrayOperationMap<T,MapFunction,InRange,OutRange>::setInternalLinks() const
    { }

    template <typename T, class MapFunction, class InRange, class OutRange>
    const T& MultiArrayOperationMap<T,MapFunction,InRange,OutRange>::get() const
    {
	return mRoot.get();
    }

    template <typename T, class MapFunction, class InRange, class OutRange>
    T& MultiArrayOperationMap<T,MapFunction,InRange,OutRange>::get()
    {
	return mRoot.get();
    }
        
    /*****************************
     *   MultiArrayOperation     *
     *****************************/

    template <size_t N>
    struct TupleIndicesLinker
    {
	template <class IndexTuple>
	static void linkTupleIndicesTo(IndexTuple& itp, IndefinitIndexBase* target)
	{
	    std::get<N>(itp).linkIndicesTo(target);
	    TupleIndicesLinker<N-1>::linkTupleIndicesTo(itp, target);
	}
    };
    
    template <>
    struct TupleIndicesLinker<0>
    {
	template <class IndexTuple>
	static void linkTupleIndicesTo(IndexTuple& itp, IndefinitIndexBase* target)
	{
	    std::get<0>(itp).linkIndicesTo(target);
	}
    };

    template <size_t N>
    struct OperationCall
    {
	template <class Operation, class Tuple, class... MAOps>
	static auto callOperation(Operation& op, const Tuple& tp, const MAOps&... args)
	    -> decltype(OperationCall<N-1>::template callOperation(op, tp, std::get<N>(tp), args...))
	{
	    return OperationCall<N-1>::template callOperation(op, tp, std::get<N>(tp), args...);
	}

	template <class Operation, class Tuple, class... MAOps>
	static auto callOperation(const Operation& op, const Tuple& tp, const MAOps&... args)
	    -> decltype(OperationCall<N-1>::template callOperation(op, tp, std::get<N>(tp), args...))
	{
	    return OperationCall<N-1>::template callOperation(op, tp, std::get<N>(tp), args...);
	}
    };

    template <>
    struct OperationCall<0>
    {
	template <class Operation, class Tuple, class... MAOps>
	static auto callOperation(Operation& op, const Tuple& tp, const MAOps&... args)
	    -> decltype(op(std::get<0>(tp).get(), args.get()...))
	{
	    return op(std::get<0>(tp).get(), args.get()...);
	    /*
	    return op(static_cast<typename Operation::result_type>( std::get<0>(tp).get() ),
		      static_cast<typename Operation::result_type>( args.get() )...);
	    */
	}

	template <class Operation, class Tuple, class... MAOps>
	static auto callOperation(const Operation& op, const Tuple& tp, const MAOps&... args)
	    -> decltype(op(std::get<0>(tp).get(), args.get()...))
	{
	    return op(std::get<0>(tp).get(), args.get()...);
	}
    };

    template <size_t N>
    struct LinkedIndexGetter
    {
	template <class Tuple>
	static IndefinitIndexBase* getLinked(const Tuple& optuple,
					     const std::string& name,
					     IndefinitIndexBase* current)
	{
	    if(current == nullptr){
		current = std::get<N>(optuple).getLinked(name);
		LinkedIndexGetter<N-1>::getLinked(optuple, name, current);
	    }
	    return current;
	}
    };

    template <>
    struct LinkedIndexGetter<0>
    {
	template <class Tuple>
	static IndefinitIndexBase* getLinked(const Tuple& optuple,
					     const std::string& name,
					     IndefinitIndexBase* current)
	{
	    if(current == nullptr){
		current = std::get<0>(optuple).getLinked(name);
	    }
	    return current;
	}
    };
    
    template <typename T, class Operation, class... MAOps>
    MultiArrayOperation<T,Operation,MAOps...>::
    MultiArrayOperation(Operation& op, const MAOps&... args) :
	mOp(op),
	mArgs(std::make_tuple(args...)) { /*CHECK;*/ }

    template <typename T, class Operation, class... MAOps>
    MultiArrayOperation<T,Operation,MAOps...>::
    MultiArrayOperation(const Operation& op, const MAOps&... args) :
	mOp(op),
	mArgs(std::make_tuple(args...)) { /*CHECK;*/ }

    template <typename T, class Operation, class... MAOps>
    template <class Operation2, class... MAOps2>
    MultiArrayOperation<T,Operation2,MultiArrayOperation<T,Operation,MAOps...>,MAOps2...>
    MultiArrayOperation<T,Operation,MAOps...>::operator()(Operation2& op, const MAOps2&... secs) const
    {
	return MultiArrayOperation<T,Operation2,MultiArrayOperation<T,Operation,MAOps...>,
				   MAOps2...>(op, *this, secs...);
    }

    template <typename T, class Operation, class... MAOps>
    template <class Operation2, class... MAOps2>
    MultiArrayOperation<T,Operation2,MultiArrayOperation<T,Operation,MAOps...>,MAOps2...>
    MultiArrayOperation<T,Operation,MAOps...>::operator()(const Operation2& op, const MAOps2&... secs) const
    {
	return MultiArrayOperation<T,Operation2,MultiArrayOperation<T,Operation,MAOps...>,
				   MAOps2...>(op, *this, secs...);
    }

    
    template <typename T, class Operation, class... MAOps>
    template <class Range2, class ContractOperation, class... MAOps2>
    MultiArrayContraction<T,ContractOperation,Range2,MultiArrayOperation<T,Operation,MAOps...>,MAOps2...>
    MultiArrayOperation<T,Operation,MAOps...>::
    contract(const ContractOperation& cop,
	     const std::string& indexName,
	     const MAOps2&... mao) const
    {
	typename Range2::IndexType* ind = dynamic_cast<typename Range2::IndexType*>( getLinked(indexName) );
	return MultiArrayContraction<T,ContractOperation,Range2,MultiArrayOperation<T,Operation,MAOps...>,
				     MAOps2...>(cop, *ind, *this, mao...);
    }
    
    template <typename T, class Operation, class... MAOps>
    template <class MAOp2>
    auto MultiArrayOperation<T,Operation,MAOps...>::operator+(const MAOp2& sec)
	-> decltype(operator()(std::plus<T>(), sec))
    {
	return operator()(std::plus<T>(), sec);
    }

    template <typename T, class Operation, class... MAOps>
    template <class MAOp2>
    auto MultiArrayOperation<T,Operation,MAOps...>::operator-(const MAOp2& sec)
	-> decltype(operator()(std::minus<T>(), sec))
    {
	return operator()(std::minus<T>(), sec);
    }

    template <typename T, class Operation, class... MAOps>
    template <class MAOp2>
    auto MultiArrayOperation<T,Operation,MAOps...>::operator*(const MAOp2& sec)
	-> decltype(operator()(std::multiplies<T>(), sec))
    {
	return operator()(std::multiplies<T>(), sec);
    }

    template <typename T, class Operation, class... MAOps>
    template <class MAOp2>
    auto MultiArrayOperation<T,Operation,MAOps...>::operator/(const MAOp2& sec)
	-> decltype(operator()(std::divides<T>(), sec))
    {
	return operator()(std::divides<T>(), sec);
    }
    
    template <typename T, class Operation, class... MAOps>
    size_t MultiArrayOperation<T,Operation,MAOps...>::argNum() const
    {
	return sizeof...(MAOps) + 1;
    }

    template <typename T, class Operation, class... MAOps>
    IndefinitIndexBase* MultiArrayOperation<T,Operation,MAOps...>::getLinked(const std::string& name) const
    {
	return LinkedIndexGetter<sizeof...(MAOps)-1>::getLinked(mArgs, name, nullptr);
    }
    
    template <typename T, class Operation, class... MAOps>    
    void MultiArrayOperation<T,Operation,MAOps...>::linkIndicesTo(IndefinitIndexBase* target) const
    {
	TupleIndicesLinker<sizeof...(MAOps)-1>::linkTupleIndicesTo(mArgs, target);
    }

    
    template <typename T, class Operation, class... MAOps>
    const T& MultiArrayOperation<T,Operation,MAOps...>::get() const
    {
	//CHECK;
	mVal = OperationCall<sizeof...(MAOps)-1>::
	    template callOperation(mOp, mArgs);
	return mVal;
    }

       
    /*******************************
     *   MultiArrayContraction     *
     *******************************/

    template <typename T, class ContractOperation, class Range, class... MAOps>
    MultiArrayContraction<T,ContractOperation,Range,MAOps...>::
    MultiArrayContraction(const ContractOperation& cop,
			  const typename Range::IndexType& runIndex,
			  const MAOps&... mao) :
	mOp(cop),
	mArgs(std::make_tuple(mao...)),
    	mBeginIndex(runIndex), mEndIndex(runIndex),
	mRunIndex(runIndex)
    {
	mBeginIndex.setPos(0);
	mEndIndex.setPos(mRunIndex.max());
	// DON'T link here !!
	//linkIndicesTo(&mRunIndex);
    }

    template <typename T, class ContractOperation, class Range, class... MAOps>
    MultiArrayContraction<T,ContractOperation,Range,MAOps...>::
    MultiArrayContraction(const ContractOperation& cop,
			  const typename Range::IndexType& runIndex,
			  size_t begin,
			  size_t end,
			  const MAOps&... mao) :
	mOp(cop),
	mArgs(std::make_tuple(mao...)),
	mBeginIndex(runIndex), mEndIndex(runIndex),
	mRunIndex(runIndex)
    {
	mBeginIndex.setPos(begin);
	mEndIndex.setPos(end);
	// DON'T link here !!
	//linkIndicesTo(&mRunIndex);
    }
    
    template <typename T, class ContractOperation, class Range, class... MAOps>
    template <class Operation2, class... MAOps2>
    MultiArrayOperation<T,Operation2,MultiArrayContraction<T,ContractOperation,Range,MAOps...>,MAOps2...>
    MultiArrayContraction<T,ContractOperation,Range,MAOps...>::operator()(Operation2& op, const MAOps2&... secs) const
    {
	return MultiArrayOperation<T,Operation2,MultiArrayContraction<T,ContractOperation,Range,MAOps...>,
				     MAOps2...>(op, *this, secs...);
    }

    template <typename T, class ContractOperation, class Range, class... MAOps>
    template <class Operation2, class... MAOps2>
    MultiArrayOperation<T,Operation2,MultiArrayContraction<T,ContractOperation,Range,MAOps...>,MAOps2...>
    MultiArrayContraction<T,ContractOperation,Range,MAOps...>::operator()(const Operation2& op, const MAOps2&... secs) const
    {
	return MultiArrayOperation<T,Operation2,MultiArrayContraction<T,ContractOperation,Range,MAOps...>,
				     MAOps2...>(op, *this, secs...);
    }

    
    template <typename T, class ContractOperation, class Range, class... MAOps>
    template <class Range2, class ContractOperation2, class... MAOps2>
    MultiArrayContraction<T,ContractOperation2,Range2,
			  MultiArrayContraction<T,ContractOperation,Range,MAOps...>,MAOps2...>
    MultiArrayContraction<T,ContractOperation,Range,MAOps...>::
    contract(const ContractOperation2& cop,
	     const std::string& indexName,
	     const MAOps2&... mao) const
    {
	typename Range2::IndexType* ind = dynamic_cast<typename Range2::IndexType*>( getLinked(indexName) );
	return MultiArrayContraction<T,ContractOperation2,Range2,MultiArrayContraction<T,ContractOperation,Range,MAOps...>,
				     MAOps2...>(cop, *ind, *this, mao...);
    }
    
    template <typename T, class ContractOperation, class Range, class... MAOps>
    template <class MAOp2>
    auto MultiArrayContraction<T,ContractOperation,Range,MAOps...>::operator+(const MAOp2& sec)
	-> decltype(operator()(std::plus<T>(), sec))
    {
	return operator()(std::plus<T>(), sec);
    }

    template <typename T, class ContractOperation, class Range, class... MAOps>
    template <class MAOp2>
    auto MultiArrayContraction<T,ContractOperation,Range,MAOps...>::operator-(const MAOp2& sec)
	-> decltype(operator()(std::minus<T>(), sec))
    {
	return operator()(std::minus<T>(), sec);
    }

    template <typename T, class ContractOperation, class Range, class... MAOps>
    template <class MAOp2>
    auto MultiArrayContraction<T,ContractOperation,Range,MAOps...>::operator*(const MAOp2& sec)
	-> decltype(operator()(std::multiplies<T>(), sec))
    {
	return operator()(std::multiplies<T>(), sec);
    }

    template <typename T, class ContractOperation, class Range, class... MAOps>
    template <class MAOp2>
    auto MultiArrayContraction<T,ContractOperation,Range,MAOps...>::operator/(const MAOp2& sec)
	-> decltype(operator()(std::divides<T>(), sec))
    {
	return operator()(std::divides<T>(), sec);
    }
    
    template <typename T, class ContractOperation, class Range, class... MAOps>
    size_t MultiArrayContraction<T,ContractOperation,Range,MAOps...>::argNum() const
    {
	return sizeof...(MAOps) + 1;
    }

    template <typename T, class ContractOperation, class Range, class... MAOps>
    IndefinitIndexBase* MultiArrayContraction<T,ContractOperation,Range,MAOps...>::getLinked(const std::string& name) const
    {
	return LinkedIndexGetter<sizeof...(MAOps)-1>::getLinked(mArgs, name, nullptr);
    }

    template <typename T, class ContractOperation, class Range, class... MAOps>    
    void MultiArrayContraction<T,ContractOperation,Range,MAOps...>::setInternalLinks() const
    {
	linkIndicesTo(&mRunIndex);
    }
    
    template <typename T, class ContractOperation, class Range, class... MAOps>    
    void MultiArrayContraction<T,ContractOperation,Range,MAOps...>::linkIndicesTo(IndefinitIndexBase* target) const
    {
	if(target != &mRunIndex){
	    setInternalLinks();
	}
	TupleIndicesLinker<sizeof...(MAOps)-1>::linkTupleIndicesTo(mArgs, target);
    }


    template <typename T, class ContractOperation, class Range, class... MAOps>
    const T& MultiArrayContraction<T,ContractOperation,Range,MAOps...>::get() const
    {
	//CHECK;
	mOp.reset();
	for(mRunIndex.copyPos( mBeginIndex ); mRunIndex.pos() != mEndIndex.pos(); ++mRunIndex){
	    OperationCall<sizeof...(MAOps)-1>::
		template callOperation(mOp, mArgs);
	}
	mOp.endOp();
	return mOp();
    }
    
}
