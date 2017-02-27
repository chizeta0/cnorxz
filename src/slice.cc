// -*- C++ -*-

#include "slice.h"

namespace MultiArrayTools
{

    template <typename T, class Range, class MARange>
    Slice<T,Range,MARange,Index>::
    Slice() :
	MultiArrayBase<T,Range>(ma.range()),
	mMultiArrayRef(*this) {}

    template <typename T, class Range, class MARange>
    bool Slice<T,Range,MARange,Index>::isSlice() const
    {
	return true;
    }

    template <typename T, class Range, class MARange>
    void Slice<T,Range,MARange,Index>::set(MultiArrayBase<T,MARange>& multiArrayRef, IndefinitIndexBase* MAPtr)
    {
	mMultiArrayRef = multiArrayRef;
	
	mMAPtr.reset(new typename MARange::IndexType(MAPtr));
    }
}
