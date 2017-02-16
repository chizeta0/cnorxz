
#include "mutli_array_operation.h"

namespace MultiArrayTools
{
    /*********************************
     *   MultiArrayOperationBase     *
     *********************************/
    
    template <typename T, class Range>
    template <class Range2>
    MultiArrayOperationBase<T,Range>&
    MultiArrayOperationBase<T,Range>::operator=(const MultiArrayOperationBase<T, Range2>& in)
    {
	in.linkIndicesTo(mIibPtr);
	for(*mIibPtr = mArrayRef.begin(); *mIibPtr != mArrayRef.end(); ++(*mIibPtr)){
	    // build in vectorization later
	    get() = in.get();
	}
    }

    template <typename T, class Range>
    template <class Operation, class... Ranges>
    MultiArrayOperation<Operation>
    MultiArrayOperationBase<T,Range>::operator()(Operation& op, MultiArrayOperationBase<T,Ranges>&... secs)
    {
	return MultiArrayOperationBase<T,Range>(op, secs);
    }
    
    template <typename T, class Range>
    size_t MultiArrayOperationBase<T,Range>::argNum() const
    {
	return 1;
    }

    template <typename T, class Range>
    IndefinitIndexBase* MultiArrayOperationBase<T,Range>::index()
    {
	return mIibPtr;
    }

    template <typename T, class Range>
    void MultiArrayOperationBase<T,Range>::linkIndicesTo(IndefinitIndexBase* target)
    {
	mIibPtr->linkTo(target);
    }

    template <typename T, class Range>
    T& MultiArrayOperationBase<T,Range>::get()
    {
	return mArrayRef(*mIibPtr);
    }

    template <typename T, class Range>
    const T& MultiArrayOperationBase<T,Range>::get() const
    {
	return mArrayRef(*mIibPtr);
    }

    /*****************************
     *   MultiArrayOperation     *
     *****************************/

    template <class IndexTuple, size_t N>
    void linkTupleIndicesTo(IndexTuple& itp, IndefinitIndexBase* target)
    {
	std::get<N>(itp).linkTo(target);
	linkTupleIndicesTo<N-1>(itp, target);
    }

    template <class IndexTuple>
    void linkTupleIndicesTo<0>(IndexTuple& itp, IndefinitIndexBase* target)
    {
	std::get<0>(itp).linkTo(target);
    }

    template <size_t N, class Operation, class Tuple, class... MBases>
    auto callOperation(Operation& op, Tuple& tp, MBases&... secs)
	-> decltype(callOperation(op, tp, std::get<N-1>(tp), secs...))
    {
	return callOperation(op, tp, std::get<N-1>(tp), secs...);
    }

    template <class Operation, class Tuple, class... MBases>
    auto callOperation<0>(Operation& op, Tuple& tp, MBases&... secs) -> decltype(op(secs.get()...))
    {
	return op(secs.get()...);
    }
    
    template <typename T, class Range, class Operation, class... Ranges>
    size_t MultiArrayOperation<T,Range,Operation,Ranges...>::argNum() const
    {
	return sizeof...(Ranges) + 1;
    }

    template <typename T, class Range, class Operation, class... Ranges>    
    void MultiArrayOperation<T,Range,Operation,Ranges...>::linkIndicesTo(IndefinitIndexBase* target)
    {
	mIibPtr->linkTo(target);
	linkTupleIndicesTo<sizeof...(Ranges)>(mSecs, target);
    }
    
    template <typename T, class Range, class Operation, class... Ranges>    
    T& MultiArrayOperation<T,Range,Operation,Ranges...>::get()
    {
	mVal = callOperation<sizeof...(Ranges)>(mOp, mSecs);
	return mVal;
    }

    template <typename T, class Range, class Operation, class... Ranges>    
    const T& MultiArrayOperation<T,Range,Operation,Ranges...>::get() const
    {
	mVal = callOperation<sizeof...(Ranges)>(mOp, mSecs);
	return mVal;
    }

}
