
#include "multi_array_operation.h"

namespace MultiArrayTools
{
    /*********************************
     *   MultiArrayOperationBase     *
     *********************************/

    template <typename T, class Range>
    MultiArrayOperationBase<T,Range>::
    MultiArrayOperationBase(MultiArray<T,Range>& ma,
			    const Name& nm) : mArrayRef(ma),
					      mIibPtr(new IndexType(mArrayRef.begin()))
    {
	mIibPtr->name(nm);
    }

    template <typename T, class Range>
    MultiArrayOperationBase<T,Range>::~MultiArrayOperationBase()
    {
	delete mIibPtr;
    }

    template <typename T, class Range>
    MultiArrayOperationBase<T,Range>&
    MultiArrayOperationBase<T,Range>::operator=(const MultiArrayOperationBase<T,Range>& in)
    {
	CHECK;
	in.linkIndicesTo(mIibPtr);
	for(*mIibPtr = mArrayRef.begin(); *mIibPtr != mArrayRef.end(); ++(*mIibPtr)){
	    // build in vectorization later
	    get() = in.get();
	}
	return *this;
    }
    
    template <typename T, class Range>
    template <class Range2>
    MultiArrayOperationBase<T,Range>&
    MultiArrayOperationBase<T,Range>::operator=(const MultiArrayOperationBase<T, Range2>& in)
    {
	CHECK;
	in.linkIndicesTo(mIibPtr);
	for(*mIibPtr = mArrayRef.begin(); *mIibPtr != mArrayRef.end(); ++(*mIibPtr)){
	    // build in vectorization later
	    get() = in.get();
	}
	return *this;
    }

    template <typename T, class Range>
    template <class Operation, class... Ranges>
    MultiArrayOperation<T,Range,Operation,Ranges...>
    MultiArrayOperationBase<T,Range>::operator()(Operation& op, MultiArrayOperationBase<T,Ranges>&... secs)
    {
	return MultiArrayOperation<T,Range,Operation,Ranges...>(op, secs...);
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
	return mArrayRef[*dynamic_cast<IndexType*>(mIibPtr)];
    }

    template <typename T, class Range>
    const T& MultiArrayOperationBase<T,Range>::get() const
    {
	return mArrayRef[*dynamic_cast<IndexType*>(mIibPtr)];
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
	    std::get<N>(itp).linkTo(target);
	    linkTupleIndicesTo<N-1>(itp, target);
	}
    };
    
    template <>
    struct TupleIndicesLinker<0>
    {
	template <class IndexTuple>
	static void linkTupleIndicesTo(IndexTuple& itp, IndefinitIndexBase* target)
	{
	    std::get<0>(itp).linkTo(target);
	}
    };

    template <size_t N>
    struct OperationCall
    {	
	template <class Operation, class Tuple, class... MBases>
	auto callOperation(Operation& op, Tuple& tp, MBases&... secs)
	    -> decltype(callOperation(op, tp, std::get<N-1>(tp), secs...))
	{
	    return callOperation(op, tp, std::get<N-1>(tp), secs...);
	}
    };

    template <>
    struct OperationCall<0>
    {	
	template <class Operation, class Tuple, class... MBases>
	auto callOperation(Operation& op, Tuple& tp, MBases&... secs) -> decltype(op(secs.get()...))
	{
	    return op(secs.get()...);
	}
    };
    
    template <typename T, class Range, class Operation, class... Ranges>
    size_t MultiArrayOperation<T,Range,Operation,Ranges...>::argNum() const
    {
	return sizeof...(Ranges) + 1;
    }

    template <typename T, class Range, class Operation, class... Ranges>    
    void MultiArrayOperation<T,Range,Operation,Ranges...>::linkIndicesTo(IndefinitIndexBase* target)
    {
	OB::mIibPtr->linkTo(target);
	TupleIndicesLinker<sizeof...(Ranges)>::linkTupleIndicesTo(mSecs, target);
    }
    
    template <typename T, class Range, class Operation, class... Ranges>    
    T& MultiArrayOperation<T,Range,Operation,Ranges...>::get()
    {
	mVal = OperationCall<sizeof...(Ranges)>::callOperation(mOp, mSecs);
	return mVal;
    }

    template <typename T, class Range, class Operation, class... Ranges>    
    const T& MultiArrayOperation<T,Range,Operation,Ranges...>::get() const
    {
	mVal = OperationCall<sizeof...(Ranges)>::callOperation(mOp, mSecs);
	return mVal;
    }

}
