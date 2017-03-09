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
    IndefinitIndexBase& MultiArrayOperationBase<T>::index()
    {
	return *mIibPtr;
    }
    
    /*********************************
     *   MultiArrayOperationBase     *
     *********************************/

    template <typename T, class Range>
    void MultiArrayOperationRoot<T,Range>::performAssignment(const MultiArrayOperationBase<T>& in)
    {
	in.linkIndicesTo(MAOB::mIibPtr);
	IndexType& iref = dynamic_cast<IndexType&>(*MAOB::mIibPtr);
	for(iref = mArrayRef.begin().pos(); iref != mArrayRef.end(); ++iref){
	    // build in vectorization later
	    get() = in.get();
	}
	MAOB::mIibPtr->freeLinked();
    }

    template <typename T, class Range>
    template <class RangeX>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::makeSlice(MultiArrayOperationRoot<T,RangeX>& in)
    {
	Slice<T,Range,RangeX>& sl = dynamic_cast<Slice<T,Range,RangeX>&>( mArrayRef );
	sl.set(in.mArrayRef, name(), dynamic_cast<typename RangeX::IndexType&>( in.index() ), in.name());
	return *this;
    }
    
    template <typename T, class Range>
    MultiArrayOperationRoot<T,Range>::
    MultiArrayOperationRoot(MultiArrayBase<T,Range>& ma,
			    const Name& nm) :
	MultiArrayOperationBase<T>(),
	mArrayRef(ma),
	mIndex(mArrayRef.begin()),
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
	return *this;
    }

    template <typename T, class Range>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator=(MultiArrayOperationRoot<T,Range>& in)
    {
	maketurnSlice(in);
	if(mArrayRef.isSlice() and not mArrayRef.isInit()){
	    return makeSlice(in);
	}
	performAssignment(in);
	return *this;
    }

    template <typename T, class Range>
    template <class Range2>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator=(MultiArrayOperationRoot<T,Range2>& in)
    {
	if(mArrayRef.isSlice() and not mArrayRef.isInit()){
	    return makeSlice(in);
	}
	performAssignment(in);
	return *this;
    }
    
    template <typename T, class Range>
    template <class Operation, class... MAOps>
    MultiArrayOperationRoot<T,Range>&
    MultiArrayOperationRoot<T,Range>::operator=(const MultiArrayOperation<T,Operation,MAOps...>& in)
    {
	if(mArrayRef.isSlice() and not mArrayRef.isInit()){
	    // NO SLICE CREATION !!! (total array not initialized!!)
	    // throw !
	    assert(0);
	}
   	performAssignment(in);
	return *this;
    }

    template <typename T, class Range>
    template <class Operation, class... MAOps>
    MultiArrayOperation<T,Operation,MultiArrayOperationRoot<T,Range>, MAOps...>
    MultiArrayOperationRoot<T,Range>::operator()(Operation& op, const MAOps&... secs)
    {
	return MultiArrayOperation<T,Operation,MultiArrayOperationRoot<T,Range>, MAOps...>(op, *this, secs...);
    }

    template <typename T, class Range>
    template <class Operation, class... MAOps>
    MultiArrayOperation<T,Operation,MultiArrayOperationRoot<T,Range>, MAOps...>
    MultiArrayOperationRoot<T,Range>::operator()(const Operation& op, const MAOps&... secs)
    {
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
	return operator()(std::divides<T>(), sec);
    }

    template <typename T, class Range>
    MultiArrayOperationRoot<T,Range> MultiArrayOperationRoot<T,Range>::copyThis()
    {
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
	return (*this) = copyThis() / sec;
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
    MultiArrayOperation<T,Operation,MAOps...>::operator()(Operation2& op, const MAOps2&... secs)
    {
	return MultiArrayOperation<T,Operation2,MultiArrayOperation<T,Operation,MAOps...>,
				   MAOps2...>(op, *this, secs...);
    }

    template <typename T, class Operation, class... MAOps>
    template <class Operation2, class... MAOps2>
    MultiArrayOperation<T,Operation2,MultiArrayOperation<T,Operation,MAOps...>,MAOps2...>
    MultiArrayOperation<T,Operation,MAOps...>::operator()(const Operation2& op, const MAOps2&... secs)
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
    
    template <typename T, class Operation, class... MAOps>    
    T& MultiArrayOperation<T,Operation,MAOps...>::get()
    {
	mVal = OperationCall<sizeof...(MAOps)-1>::
	    template callOperation(mOp, mArgs);
	return mVal;
    }

    template <typename T, class Operation, class... MAOps>    
    const T& MultiArrayOperation<T,Operation,MAOps...>::get() const
    {
	mVal = OperationCall<sizeof...(MAOps)-1>::
	    template callOperation(mOp, mArgs);
	return mVal;
    }
}
