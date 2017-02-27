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
    template <typename T, class Range, class MARange/*, class Index*/>
    class Slice : public MultiArrayBase<T,Range> // yes, 'Range' is correct !!!
    {
    public:

	typedef MultiArrayBase<T,Range> MAB;

	Slice(const Range& range);

	T& operator[](const typename Range::IndexType& i) override;
	const T& operator[](const typename Range::IndexType& i) const override;
	
	//Slice& setSlicePos(const Index& slicePos);
	
	// link given Index to mMAPtr which is index of total array
	auto begin() override -> decltype(Range().begin()); 
	auto end() override -> decltype(Range().end());

	virtual bool isSlice() const override;

	void set(MultiArrayBase<T,MARange>& multiArrayRef,
		 const Name& ownNm,
		 IndefinitIndexBase* MAPtr,
		 const Name& MANm);
	
    private:

	MultiArrayBase<T,MARange>& mMultiArrayRef;
	std::shared_ptr<IndefinitIndexBase> mMAPtr; // idx ptr for original MA Range
	std::shared_ptr<IndefinitIndexBase> mOwnPtr; // idx ptr for own Range
	//Index mSlicePos;
    };
    
    
}

#include "slice.cc"

#endif
