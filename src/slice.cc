// -*- C++ -*-

#include "slice.h"

namespace MultiArrayTools
{

    template <typename T, class Range, class MARange>
    Slice<T,Range,MARange>::
    Slice(const Range& range) :
	MultiArrayBase<T,Range>(range) {}

    template <typename T, class Range, class MARange>
    bool Slice<T,Range,MARange>::isSlice() const
    {
	return true;
    }

    template <typename T, class Range, class MARange>
    void Slice<T,Range,MARange>::set(MultiArrayBase<T,MARange>& multiArrayRef,
				     const Name& ownNm, // for correct linkage
				     const typename MARange::IndexType& MAIdx, // for desired slice position
				     const Name& MANm) // for correct linkage)
    {
	MAB::mInit = true;
	mMultiArrayPtr = &multiArrayRef;
	mMAIdx = MAIdx;
	mOwnIdx = MAB::mRange->begin();
	mMAIdx.name(MANm);
	mOwnIdx.name(ownNm);
	mMAIdx.linkTo(&mOwnIdx);
    }

    template <typename T, class Range, class MARange>
    auto Slice<T,Range,MARange>::begin() const -> decltype(Range().begin())
    {
	return MAB::mRange->begin();
    }

    template <typename T, class Range, class MARange>
    auto Slice<T,Range,MARange>::end() const -> decltype(Range().end())
    {
	return MAB::mRange->end();
    }

    template <typename T, class Range, class MARange>
    T& Slice<T,Range,MARange>::operator[](const typename Range::IndexType& i)
    {
	mOwnIdx.copyPos(i);
	//mOwnIdx = i.pos();
	return (*mMultiArrayPtr)[ mMAIdx ];
    }

    template <typename T, class Range, class MARange>
    const T& Slice<T,Range,MARange>::operator[](const typename Range::IndexType& i) const
    {
	mOwnIdx.copyPos(i);
	//mOwnIdx = i.pos();
	return (*mMultiArrayPtr)[ mMAIdx ];
    }

    template <typename T, class Range, class MARange>
    ConstSlice<T,Range,MARange>::
    ConstSlice(const Range& range) :
	MultiArrayBase<T,Range>(range) {}

    template <typename T, class Range, class MARange>
    bool ConstSlice<T,Range,MARange>::isSlice() const
    {
	return true;
    }

    template <typename T, class Range, class MARange>
    void ConstSlice<T,Range,MARange>::set(const MultiArrayBase<T,MARange>& multiArrayRef,
					  const Name& ownNm, // for correct linkage
					  const typename MARange::IndexType& MAIdx, // for desired slice position
					  const Name& MANm) // for correct linkage)
    {
	MAB::mInit = true;
	mMultiArrayPtr = &multiArrayRef;
	mMAIdx = MAIdx;
	mOwnIdx = MAB::mRange->begin();
	mMAIdx.name(MANm);
	mOwnIdx.name(ownNm);
	mMAIdx.linkTo(&mOwnIdx);
    }

    template <typename T, class Range, class MARange>
    auto ConstSlice<T,Range,MARange>::begin() const -> decltype(Range().begin())
    {
	return MAB::mRange->begin();
    }

    template <typename T, class Range, class MARange>
    auto ConstSlice<T,Range,MARange>::end() const -> decltype(Range().end())
    {
	return MAB::mRange->end();
    }

    template <typename T, class Range, class MARange>
    const T& ConstSlice<T,Range,MARange>::operator[](const typename Range::IndexType& i) const
    {
	mOwnIdx.copyPos(i);
	//mOwnIdx = i.pos();
	return (*mMultiArrayPtr)[ mMAIdx ];
    }

    
    template <typename T, class Range, class MARange>
    T& ConstSlice<T,Range,MARange>::operator[](const typename Range::IndexType& i)
    {
	assert(0);
	mOwnIdx.copyPos(i);
	//mOwnIdx = i.pos();
	return x;
    }
}
