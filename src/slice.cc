// -*- C++ -*-

#include "slice.h"

namespace MultiArrayTools
{

    template <typename T, class Range, class MARange>
    Slice<T,Range,MARange,Index>::
    Slice(const Range& range) :
	MultiArrayBase<T,Range>(range),
	mMultiArrayRef(*this) {}

    template <typename T, class Range, class MARange>
    bool Slice<T,Range,MARange,Index>::isSlice() const
    {
	return true;
    }

    template <typename T, class Range, class MARange>
    void Slice<T,Range,MARange,Index>::set(MultiArrayBase<T,MARange>& multiArrayRef,
					   const Name& ownNm, // for correct linkage
					   IndefinitIndexBase* MAPtr, // for desired slice position
					   const Name& MANm) // for correct linkage)
    {
	mMultiArrayRef = multiArrayRef;
	mMAPtr.reset(new typename MARange::IndexType(*MAPtr));
	mOwnPtr.reset(new typename Range::IndexType());
	mMAPtr->name(MANm);
	mOwnPtr->name(ownNm);
	mMAPtr->linkTo(mOwnPtr);
    }
}
