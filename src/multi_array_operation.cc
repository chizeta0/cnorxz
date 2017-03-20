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
    void MultiArrayOperationBase<T>::freeIndex()
    {
	mIibPtr->freeLinked();
    }
    
    /*********************************
     *   MultiArrayOperationBase     *
     *********************************/

    template <typename T, class Range>
    void MultiArrayOperationRoot<T,Range>::performAssignment(const MultiArrayOperationBase<T>& in)
    {
#error "WRITE MAOR INTRINSIC CONTRACT FUNCTION"
	//CHECK;
	in.linkIndicesTo(MAOB::mIibPtr);
	//CHECK;
	IndexType& iref = dynamic_cast<IndexType&>(*MAOB::mIibPtr);
	//CHECK;
	const size_t endPos = mArrayRef.endIndex().pos();
	std::cout << "assignment: " << endPos << " elements" << std::endl;
	for(iref = mArrayRef.beginIndex().pos(); iref != mArrayRef.endIndex(); ++iref){
	    std::cout << iref.pos() << '\r' << std::flush;
	    get() = in.get();
	}
	//CHECK;
	MAOB::mIibPtr->freeLinked();
    }

    template <typename T, class Range>
    template <class RangeX>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::makeSlice(MultiArrayOperationRoot<T,RangeX>& in)
    {
	Slice<T,Range,RangeX>& sl = dynamic_cast<Slice<T,Range,RangeX>&>( mArrayRef );
	sl.set(in.mArrayRef, name(), dynamic_cast<const typename RangeX::IndexType&>( in.index() ), in.name());
	return *this;
    }

    
    template <typename T, class Range>
    template <class RangeX>
    const MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::makeConstSlice(const MultiArrayOperationRoot<T,RangeX>& in)
    {
	Slice<T,Range,RangeX>& sl = dynamic_cast<Slice<T,Range,RangeX>&>( mArrayRef );
	sl.setConst(in.mArrayRef, name(), dynamic_cast<const typename RangeX::IndexType&>( in.index() ), in.name());
	return *this;
    }
    
    
    // CONST SLICE !!!!!
    
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
	//CHECK;
	maketurnSlice(in);
	if(mArrayRef.isSlice() and not mArrayRef.isInit()){
	    return makeSlice(in);
	}
	performAssignment(in);
	freeIndex();
	return *this;
    }

    template <typename T, class Range>
    template <class Range2>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator=(MultiArrayOperationRoot<T,Range2>& in)
    {
	//CHECK;
	if(mArrayRef.isSlice() and not mArrayRef.isInit()){
	    return makeSlice(in);
	}
	performAssignment(in);
	freeIndex();
	return *this;
    }

    
    template <typename T, class Range>
    template <class Range2>
    const MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator=(const MultiArrayOperationRoot<T,Range2>& in)
    {
	//CHECK;
	if(mArrayRef.isSlice() and not mArrayRef.isInit()){
	    //CHECK;
	    return makeConstSlice(in);
	}
	performAssignment(in);
	freeIndex();
	return *this;
    }
    
    template <typename T, class Range>
    template <class Operation, class... MAOps>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator=(const MultiArrayOperation<T,Operation,MAOps...>& in)
    {
	//CHECK;
	if(mArrayRef.isSlice() and not mArrayRef.isInit()){
	    // NO SLICE CREATION !!! (total array not initialized!!)
	    // throw !
	    assert(0);
	}
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
    void MultiArrayOperationRoot<T,Range>::linkIndicesTo(IndefinitIndexBase* target) const
    {
	MAOB::mIibPtr->linkTo(target);
    }

    template <typename T, class Range>
    T& MultiArrayOperationRoot<T,Range>::get()
    {
	//return mArrayRef[mIndex];
	return mArrayRef[*dynamic_cast<IndexType*>(MAOB::mIibPtr)];
    }

    template <typename T, class Range>
    const T& MultiArrayOperationRoot<T,Range>::get() const
    {
	//return mArrayRef[mIndex];
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
    void MultiArrayOperationRoot<T,Range>::freeIndex()
    {
	MAOB::mIibPtr->freeLinked();
	mIndex = mArrayRef.beginIndex();
	MAOB::mIibPtr = &mIndex;
	MAOB::mIibPtr->name(mNm);
    }

    /**************************************
     *   ConstMultiArrayOperationBase     *
     **************************************/

    /*
    template <typename T, class Range>
    template <class RangeX>
    const ConstMultiArrayOperationRoot<T,Range>&
    ConstMultiArrayOperationRoot<T,Range>::makeConstSlice(const ConstMultiArrayOperationRoot<T,RangeX>& in)
    {
	ConstSlice<T,Range,RangeX>& sl = dynamic_cast<ConstSlice<T,Range,RangeX>&>( mArrayRef );
	sl.set(in.mArrayRef, name(), dynamic_cast<const typename RangeX::IndexType&>( in.index() ), in.name());
	return *this;
    }

    template <typename T, class Range>
    template <class RangeX>
    const ConstMultiArrayOperationRoot<T,Range>&
    ConstMultiArrayOperationRoot<T,Range>::makeConstSlice(const MultiArrayOperationRoot<T,RangeX>& in)
    {
	ConstSlice<T,Range,RangeX>& sl = dynamic_cast<ConstSlice<T,Range,RangeX>&>( mArrayRef );
	sl.set(in.mArrayRef, name(), dynamic_cast<const typename RangeX::IndexType&>( in.index() ), in.name());
	return *this;
    }
    */
    
    // CONST SLICE !!!!!
    
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

    /*
    template <typename T, class Range>
    const ConstMultiArrayOperationRoot<T,Range>&
    ConstMultiArrayOperationRoot<T,Range>::operator=(const ConstMultiArrayOperationRoot<T,Range>& in)
    {
	//CHECK;
	if(mArrayRef.isSlice() and not mArrayRef.isInit()){
	    return makeConstSlice(in);
	}
	assert(0);
	return *this;
    }

    template <typename T, class Range>
    template <class Range2>
    const ConstMultiArrayOperationRoot<T,Range>&
    ConstMultiArrayOperationRoot<T,Range>::operator=(const ConstMultiArrayOperationRoot<T,Range2>& in)
    {
	//CHECK;
	if(mArrayRef.isSlice() and not mArrayRef.isInit()){
	    //CHECK;
	    return makeConstSlice(in);
	}
	assert(0);
	return *this;
    }

    template <typename T, class Range>
    template <class Range2>
    const ConstMultiArrayOperationRoot<T,Range>&
    ConstMultiArrayOperationRoot<T,Range>::operator=(const MultiArrayOperationRoot<T,Range2>& in)
    {
	//CHECK;
	if(mArrayRef.isSlice() and not mArrayRef.isInit()){
	    //CHECK;
	    return makeConstSlice(in);
	}
	assert(0);
	return *this;
    }
    */
    template <typename T, class Range>
    template <class Operation, class... MAOps>
    MultiArrayOperation<T,Operation,ConstMultiArrayOperationRoot<T,Range>, MAOps...>
    ConstMultiArrayOperationRoot<T,Range>::operator()(const Operation& op, const MAOps&... secs) const
    {
	//CHECK;
	return MultiArrayOperation<T,Operation,ConstMultiArrayOperationRoot<T,Range>, MAOps...>(op, *this, secs...);
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
    void ConstMultiArrayOperationRoot<T,Range>::linkIndicesTo(IndefinitIndexBase* target) const
    {
	MAOB::mIibPtr->linkTo(target);
    }

    template <typename T, class Range>
    const T& ConstMultiArrayOperationRoot<T,Range>::get() const
    {
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
    void ConstMultiArrayOperationRoot<T,Range>::freeIndex()
    {
	MAOB::mIibPtr->freeLinked();
	mIndex = mArrayRef.beginIndex();
	MAOB::mIibPtr = &mIndex;
	MAOB::mIibPtr->name(mNm);
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
	}

	template <class Operation, class Tuple, class... MAOps>
	static auto callOperation(const Operation& op, const Tuple& tp, const MAOps&... args)
	    -> decltype(op(std::get<0>(tp).get(), args.get()...))
	{
	    return op(std::get<0>(tp).get(), args.get()...);
	}
    };

    template <typename T, class Operation, class... MAOps>
    MultiArrayOperation<T,Operation,MAOps...>::
    MultiArrayOperation(Operation& op, const MAOps&... args) :
	mOp(op),
	mArgs(std::make_tuple(args...)) {}

    template <typename T, class Operation, class... MAOps>
    MultiArrayOperation<T,Operation,MAOps...>::
    MultiArrayOperation(const Operation& op, const MAOps&... args) :
	mOp(op),
	mArgs(std::make_tuple(args...)) {}

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
    void MultiArrayOperation<T,Operation,MAOps...>::linkIndicesTo(IndefinitIndexBase* target) const
    {
	TupleIndicesLinker<sizeof...(MAOps)-1>::linkTupleIndicesTo(mArgs, target);
    }

    /*
    template <typename T, class Operation, class... MAOps>    
    T& MultiArrayOperation<T,Operation,MAOps...>::get()
    {
	mVal = OperationCall<sizeof...(MAOps)-1>::
	    template callOperation(mOp, mArgs);
	return mVal;
	}*/

    template <typename T, class Operation, class... MAOps>    
    const T& MultiArrayOperation<T,Operation,MAOps...>::get() const
    {
	mVal = OperationCall<sizeof...(MAOps)-1>::
	    template callOperation(mOp, mArgs);
	return mVal;
    }
}
