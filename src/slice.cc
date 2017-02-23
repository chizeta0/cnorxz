// -*- C++ -*-

#include "slice.h"

namespace MultiArrayTools
{

    template <typename T, class Range, class MARange, class Index>
    Slice<T,Range,MARange,Index>::
    Slice(const Range& range, const Index& slicePos) :
	MA::mRange(new Range(range)),
	mSlicePos(slicePos) {}

    template <typename T, class Range, class MARange, class Index>
    Slice<T,Range,MARange,Index>& Slice<T,Range,MARange,Index>::setSlicePos(const Index& slicePos)
    {
	mSlicePos = slicePos;
	mMAPtr->linkTo(&mSlicePos);
	return *this;
    }

    template <typename T, class Range, class MARange, class Index>
    bool Slice<T,Range,MARange,Index>::isSlice() const
    {
	return true;
    }

    template <typename T, class Range, class MARange, class Index>
    void Slice<T,Range,MARange,Index>::setPtr(IndefinitIndexBase* MAPtr,
					      IndefinitIndexBase* ownPtr)
    {
	mMAPtr.reset(new typename MARange::IndexType(MAPtr));
	mOwnPtr.reset(new typename Range::IndexType(ownPtr));
    }
}
