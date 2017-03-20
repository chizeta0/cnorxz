// -*- C++ -*-

#include "slice.h"

namespace MultiArrayTools
{

    template <typename T, class Range, class MARange>
    Slice<T,Range,MARange>::
    Slice(const Range& range,
	  const Name& ownNm, // for correct linkage
	  MutableMultiArrayBase<T,MARange>& multiArrayRef,
	  const typename MARange::IndexType& MAIdx, // for desired slice position
	  const Name& MANm) :
	MutableMultiArrayBase<T,Range>(range),
	mOwnName(ownNm), mMAName(MANm), mMultiArrayPtr(&multiArrayRef),
	mMAIdx(MAIdx), mBeginIndex(range.begin()), mEndIndex(range.end())
    {
	MAB::mInit = true;
	mOwnIdx = MAB::mRange->begin();
	mMAIdx.name(MANm);
	mOwnIdx.name(ownNm);
	mMAIdx.linkTo(&mOwnIdx);
    }

    template <typename T, class Range, class MARange>
    Slice<T,Range,MARange>::
    Slice(const Range& range,
	  const Name& ownNm,
	  MultiArrayOperationRoot<T,MARange>& mor,
	  const typename MARange::IndexType& MAIdx) :
	MutableMultiArrayBase<T,Range>(range),
	mOwnName(ownNm), mMAName(mor.name()), mMultiArrayPtr(&(*mor)),
	mMAIdx(MAIdx), mBeginIndex(range.begin()), mEndIndex(range.end())
    {
	MAB::mInit = true;
	mOwnIdx = MAB::mRange->begin();
	mMAIdx.name(mMAName);
	mOwnIdx.name(ownNm);
	mMAIdx.linkTo(&mOwnIdx);
    }
    
    template <typename T, class Range, class MARange>
    bool Slice<T,Range,MARange>::isSlice() const
    {
	return true;
    }
    
    template <typename T, class Range, class MARange>
    auto Slice<T,Range,MARange>::beginIndex() const -> decltype(Range().begin())
    {
	return mBeginIndex;
    }

    template <typename T, class Range, class MARange>
    auto Slice<T,Range,MARange>::endIndex() const -> decltype(Range().end())
    {
	return mEndIndex;
    }

    // slow!!
    template <typename T, class Range, class MARange>
    T& Slice<T,Range,MARange>::operator[](const typename Range::IndexType& i)
    {
	mOwnIdx.copyPos(i);
	return (*mMultiArrayPtr)[ mMAIdx ];
    }

    // slow!!
    template <typename T, class Range, class MARange>
    const T& Slice<T,Range,MARange>::operator[](const typename Range::IndexType& i) const
    {
	mOwnIdx.copyPos(i);
	return (*mMultiArrayPtr)[ mMAIdx ];
    }
    
    template <typename T, class Range, class MARange>
    bool Slice<T,Range,MARange>::isConst() const
    {
	return false;
    }

    template <typename T, class Range, class MARange>
    void Slice<T,Range,MARange>::link(IndefinitIndexBase* iibPtr) const
    {
	mMAIdx.linkTo(iibPtr);
    }
    
    template <typename T, class Range, class MARange>
    ConstSlice<T,Range,MARange>::
    ConstSlice(const Range& range,
	       const Name& ownNm, // for correct linkage
	       const MultiArrayBase<T,MARange>& multiArrayRef,
	       const typename MARange::IndexType& MAIdx, // for desired slice position
	       const Name& MANm) :
	MultiArrayBase<T,Range>(range),
	mMultiArrayPtr(&multiArrayRef),
	mMAIdx(MAIdx), mBeginIndex(range.begin()), mEndIndex(range.end())	
    {
	MAB::mInit = true;
	mOwnIdx = MAB::mRange->begin();
	mMAIdx.name(MANm);
	mOwnIdx.name(ownNm);
	mMAIdx.linkTo(&mOwnIdx);
    }

    template <typename T, class Range, class MARange>
    ConstSlice<T,Range,MARange>::
    ConstSlice(const Range& range,
	  const Name& ownNm,
	  const ConstMultiArrayOperationRoot<T,MARange>& mor,
	  const typename MARange::IndexType& MAIdx) :
	MultiArrayBase<T,Range>(range),
	mMultiArrayPtr(&(*mor)),
	mMAIdx(MAIdx), mBeginIndex(range.begin()), mEndIndex(range.end())
    {
	MAB::mInit = true;
	mOwnIdx = MAB::mRange->begin();
	mMAIdx.name(mor.name());
	mOwnIdx.name(ownNm);
	mMAIdx.linkTo(&mOwnIdx);
    }
    
    template <typename T, class Range, class MARange>
    ConstSlice<T,Range,MARange>::
    ConstSlice(const Slice<T,Range,MARange>& slice) :
	MultiArrayBase<T,Range>(slice.range()),
	mOwnIdx(slice.mOwnIdx),
	mMAIdx(slice.mMAIdx), mBeginIndex(slice.mBeginIndex), mEndIndex(slice.mEndIndex)
    {
	if(slice.mInit){
	    MAB::mInit = true;
	    mMultiArrayPtr = slice.mConstMultiArrayPtr;
	    mMAIdx = slice.mMAIdx;
	    mOwnIdx = MAB::mRange->begin();
	    mMAIdx.name(slice.mMAName);
	    mOwnIdx.name(slice.mOwnName);
	    mMAIdx.linkTo(&mOwnIdx);
	}
    }
	
    
    template <typename T, class Range, class MARange>
    bool ConstSlice<T,Range,MARange>::isSlice() const
    {
	return true;
    }

    template <typename T, class Range, class MARange>
    bool ConstSlice<T,Range,MARange>::isConst() const
    {
	return true;
    }

    template <typename T, class Range, class MARange>
    void ConstSlice<T,Range,MARange>::link(IndefinitIndexBase* iibPtr) const
    {
	mMAIdx.linkTo(iibPtr);
    }

    template <typename T, class Range, class MARange>
    auto ConstSlice<T,Range,MARange>::beginIndex() const -> decltype(Range().begin())
    {
	return mBeginIndex;
    }

    template <typename T, class Range, class MARange>
    auto ConstSlice<T,Range,MARange>::endIndex() const -> decltype(Range().end())
    {
	return mEndIndex;
    }

    // slow!!
    template <typename T, class Range, class MARange>
    const T& ConstSlice<T,Range,MARange>::operator[](const typename Range::IndexType& i) const
    {
	mOwnIdx.copyPos(i);
	//mOwnIdx = i.pos();
	return (*mMultiArrayPtr)[ mMAIdx ];
    }
}
