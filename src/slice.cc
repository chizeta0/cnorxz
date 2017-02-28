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
					   IndefinitIndexBase* MAPtr, // for desired slice position
					   const Name& MANm) // for correct linkage)
    {
	mMultiArrayPtr = &multiArrayRef;
	mMAPtr.reset(new typename MARange::IndexType( dynamic_cast<typename MARange::IndexType&>( *MAPtr ) ) );
	mOwnPtr.reset(new typename Range::IndexType());
	mMAPtr->name(MANm);
	mOwnPtr->name(ownNm);
	mMAPtr->linkTo(mOwnPtr.get());
    }

    template <typename T, class Range, class MARange>
    auto Slice<T,Range,MARange>::begin() -> decltype(Range().begin())
    {
	return MAB::mRange->begin();
    }

    template <typename T, class Range, class MARange>
    auto Slice<T,Range,MARange>::end() -> decltype(Range().end())
    {
	return MAB::mRange->end();
    }

    template <typename T, class Range, class MARange>
    T& Slice<T,Range,MARange>::operator[](const typename Range::IndexType& i)
    {
	dynamic_cast<typename Range::IndexType&>(*mOwnPtr).copyPos(i);
	return (*mMultiArrayPtr)[ dynamic_cast<typename MARange::IndexType&>( *mMAPtr ) ];
    }

    template <typename T, class Range, class MARange>
    const T& Slice<T,Range,MARange>::operator[](const typename Range::IndexType& i) const
    {
	dynamic_cast<typename Range::IndexType&>(*mOwnPtr).copyPos(i);
	return (*mMultiArrayPtr)[ dynamic_cast<typename MARange::IndexType&>( *mMAPtr ) ];
    }
}
