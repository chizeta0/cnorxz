
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
					      //mIndex(mArrayRef.begin()),
					      mIibPtr(new IndexType(mArrayRef.begin())),
					      mNm(nm)
    {
	mIibPtr->name(nm);
	//mIndex.name(nm);
    }

    template <typename T, class Range>
    MultiArrayOperationBase<T,Range>::~MultiArrayOperationBase()
    {
	//delete mIibPtr;
    }

    template <typename T, class Range>
    MultiArrayOperationBase<T,Range>&
    MultiArrayOperationBase<T,Range>::operator=(const MultiArrayOperationBase<T,Range>& in)
    {
	in.linkIndicesTo(mIibPtr);
	IndexType& iref = dynamic_cast<IndexType&>(*mIibPtr);
	for(iref = mArrayRef.begin().pos(); iref != mArrayRef.end(); ++iref){
	    // build in vectorization later
	    get() = in.get();
	}
	mIibPtr->freeLinked();
	return *this;
    }
    
    template <typename T, class Range>
    template <class Range2>
    MultiArrayOperationBase<T,Range>&
    MultiArrayOperationBase<T,Range>::operator=(const MultiArrayOperationBase<T, Range2>& in)
    {
	in.linkIndicesTo(mIibPtr);
	IndexType& iref = dynamic_cast<IndexType&>(*mIibPtr);
	if(mArrayRef.isSlice()){
	    linkSlice(&in.index(), mIibPtr);
	    return *this;
	}
	for(iref = mArrayRef.begin().pos(); iref != mArrayRef.end(); ++iref){
	    // build in vectorization later
	    get() = in.get();
	}
	mIibPtr->freeLinked();
	return *this;
    }

    template <typename T, class Range>
    template <class Operation, class... Ranges>
    MultiArrayOperation<T,Range,Operation,Ranges...>
    MultiArrayOperationBase<T,Range>::operator()(Operation& op, const MultiArrayOperationBase<T,Ranges>&... secs)
    {
	return MultiArrayOperation<T,Range,Operation,Ranges...>(mArrayRef, mNm, op, secs...);
    }

    template <typename T, class Range>
    template <class Operation, class... Ranges>
    MultiArrayOperation<T,Range,Operation,Ranges...>
    MultiArrayOperationBase<T,Range>::operator()(const Operation& op,
						 const MultiArrayOperationBase<T,Ranges>&... secs)
    {
	return MultiArrayOperation<T,Range,Operation,Ranges...>(mArrayRef, mNm, op, secs...);
    }

    template <typename T, class Range>
    template <class Range2>
    MultiArrayOperation<T,Range,std::plus<T>,Range2>
    MultiArrayOperationBase<T,Range>::operator+(const MultiArrayOperationBase<T,Range2>& sec)
    {
	return operator()(std::plus<T>(), sec);
    }

    template <typename T, class Range>
    template <class Range2>
    MultiArrayOperation<T,Range,std::minus<T>,Range2>
    MultiArrayOperationBase<T,Range>::operator-(const MultiArrayOperationBase<T,Range2>& sec)
    {
	return operator()(std::minus<T>(), sec);
    }

    template <typename T, class Range>
    template <class Range2>
    MultiArrayOperation<T,Range,std::multiplies<T>,Range2>
    MultiArrayOperationBase<T,Range>::operator*(const MultiArrayOperationBase<T,Range2>& sec)
    {
	return operator()(std::multiplies<T>(), sec);
    }

    template <typename T, class Range>
    template <class Range2>
    MultiArrayOperation<T,Range,std::divides<T>,Range2>
    MultiArrayOperationBase<T,Range>::operator/(const MultiArrayOperationBase<T,Range2>& sec)
    {
	return operator()(std::divides<T>(), sec);
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
    void MultiArrayOperationBase<T,Range>::linkIndicesTo(IndefinitIndexBase* target) const
    {
	mIibPtr->linkTo(target);
    }

    template <typename T, class Range>
    T& MultiArrayOperationBase<T,Range>::get()
    {
	//return mArrayRef[mIndex];
	return mArrayRef[*dynamic_cast<IndexType*>(mIibPtr)];
    }

    template <typename T, class Range>
    const T& MultiArrayOperationBase<T,Range>::get() const
    {
	//return mArrayRef[mIndex];
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
	    std::get<N>(itp).linkIndicesTo(target);
	    linkTupleIndicesTo<N-1>(itp, target);
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
	template <typename T, class Operation, class Tuple, class... MBases>
	static auto callOperation(Operation& op, const Tuple& tp, const T& first, const MBases&... secs)
	    -> decltype(OperationCall<N-1>::template callOperation(op, tp, std::get<N>(tp), secs...))
	{
	    return OperationCall<N-1>::template callOperation(op, tp, first, std::get<N>(tp), secs...);
	}

	template <typename T, class Operation, class Tuple, class... MBases>
	static auto callOperation(const Operation& op, const Tuple& tp, const T& first, const MBases&... secs)
	    -> decltype(OperationCall<N-1>::template callOperation(op, tp, std::get<N>(tp), secs...))
	{
	    return OperationCall<N-1>::template callOperation(op, tp, first, std::get<N>(tp), secs...);
	}
    };

    template <>
    struct OperationCall<0>
    {
	template <typename T, class Operation, class Tuple, class... MBases>
	static auto callOperation(Operation& op, const Tuple& tp, const T& first, const MBases&... secs)
	    -> decltype(op(first, std::get<0>(tp).get(), secs.get()...))
	{
	    return op(first, std::get<0>(tp).get(), secs.get()...);
	}

	template <typename T, class Operation, class Tuple, class... MBases>
	static auto callOperation(const Operation& op, const Tuple& tp, const T& first, const MBases&... secs)
	    -> decltype(op(first, std::get<0>(tp).get(), secs.get()...))
	{
	    return op(first, std::get<0>(tp).get(), secs.get()...);
	}
    };

    template <typename T, class Range, class Operation, class... Ranges>
    MultiArrayOperation<T,Range,Operation,Ranges...>::
    MultiArrayOperation(MultiArray<T,Range>& ma, const Name& nm,
			Operation& op,
			const MultiArrayOperationBase<T,Ranges>&... secs) :
	MultiArrayOperationBase<T,Range>(ma, nm),
	mOp(op),
	mSecs(std::make_tuple(secs...)) {}

    template <typename T, class Range, class Operation, class... Ranges>
    MultiArrayOperation<T,Range,Operation,Ranges...>::
    MultiArrayOperation(MultiArray<T,Range>& ma, const Name& nm,
			const Operation& op,
			const MultiArrayOperationBase<T,Ranges>&... secs) :
	MultiArrayOperationBase<T,Range>(ma, nm),
	mOp(op),
	mSecs(std::make_tuple(secs...)) {}
    
    template <typename T, class Range, class Operation, class... Ranges>
    size_t MultiArrayOperation<T,Range,Operation,Ranges...>::argNum() const
    {
	return sizeof...(Ranges) + 1;
    }

    template <typename T, class Range, class Operation, class... Ranges>    
    void MultiArrayOperation<T,Range,Operation,Ranges...>::linkIndicesTo(IndefinitIndexBase* target) const
    {
	OB::mIibPtr->linkTo(target);
	TupleIndicesLinker<sizeof...(Ranges)-1>::linkTupleIndicesTo(mSecs, target);
    }
    
    template <typename T, class Range, class Operation, class... Ranges>    
    T& MultiArrayOperation<T,Range,Operation,Ranges...>::get()
    {
	mVal = OperationCall<sizeof...(Ranges)-1>::
	    template callOperation(mOp, mSecs,
				   //OB::mArrayRef[OB::mIndex]);
				   OB::mArrayRef[*dynamic_cast<typename OB::IndexType*>(OB::mIibPtr)]);
	return mVal;
    }

    template <typename T, class Range, class Operation, class... Ranges>    
    const T& MultiArrayOperation<T,Range,Operation,Ranges...>::get() const
    {
	mVal = OperationCall<sizeof...(Ranges)-1>::
	    template callOperation(mOp, mSecs,
				   //OB::mArrayRef[OB::mIndex]);
				   OB::mArrayRef[*dynamic_cast<typename OB::IndexType*>(OB::mIibPtr)]);
	return mVal;
    }

}
