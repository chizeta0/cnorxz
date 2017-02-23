// -*- C++ -*-

#ifndef __slice_h__
#define __slice_h__

#include <cstdlib>
#include "base_def.h"
#include "multi_array.h"

namespace MultiArrayTools
{

    // T = data type
    // Range = range of slice
    // MARange = original range of multi array of which this is the slice
    // Index = index which determines the slice position (remnant of MARange w.o. Range)
    template <typename T, class Range, class MARange, class Index>
    class Slice : public MultiArray<T,Range> // yes, Range is correct !!!
    {
	//MA::mCont has to be empty; only make use of the provided functions
    public:

	typedef MultiArray<T,MARange> MA;

	Slice(const Range& range, const Index& slicePos);

	T& operator[](const typename Range::IndexType& i) override;
	const T& operator[](const typename Range::IndexType& i) const override;
	
	Slice& setSlicePos(const Index& slicePos);
	
	// link given Index to mMAPtr which is index of total array
	auto begin() override -> decltype(Range().begin()); 
	auto end() override -> decltype(Range().end());

	virtual bool isSlice() const override;

	void setPtr(IndefinitIndexBase* MAPtr,
		    IndefinitIndexBase* ownPtr);
	
    private:

	MultiArray<T,MARange>& multiArrayRef;
	IndefinitIndexBase* mMAPtr; // idx ptr for original MA Range
	IndefinitIndexBase* mOwnPtr; // idx ptr for own Range
	Index mSlicePos;
    };
    
    
}

#include "slice.cc"

#endif
