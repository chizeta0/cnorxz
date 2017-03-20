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
    class Slice : public MutableMultiArrayBase<T,Range> // yes, 'Range' is correct !!!
    {
    public:

	typedef MultiArrayBase<T,Range> MAB;

	Slice(const Range& range,
	      const Name& ownNm,
	      MutableMultiArrayBase<T,MARange>& multiArrayRef,
	      const typename MARange::IndexType& MAIdx,
	      const Name& MANm);

	Slice(const Range& range,
	      const Name& ownNm,
	      MultiArrayOperationRoot<T,MARange>& mor,
	      const typename MARange::IndexType& MAIdx);
	
		
	virtual T& operator[](const typename Range::IndexType& i) override;
	virtual const T& operator[](const typename Range::IndexType& i) const override;
	
	// link given Index to mMAPtr which is index of total array
	virtual auto beginIndex() const -> decltype(Range().begin()) override; 
	virtual auto endIndex() const -> decltype(Range().end()) override;

	virtual bool isSlice() const override;
	virtual bool isConst() const override;

	virtual void link(IndefinitIndexBase* iibPtr) const override;
	
	template <typename U, class RangeX, class MARangeX>	
	friend class ConstSlice;
	
    private:

	Name mOwnName;
	Name mMAName;
	MutableMultiArrayBase<T,MARange>* mMultiArrayPtr = nullptr;
	mutable typename Range::IndexType mOwnIdx;
	mutable typename MARange::IndexType mMAIdx;
	typename Range::IndexType mBeginIndex;
	typename Range::IndexType mEndIndex;
    };

    template <typename T, class Range, class MARange/*, class Index*/>
    class ConstSlice : public MultiArrayBase<T,Range> // yes, 'Range' is correct !!!
    {
    public:

	typedef MultiArrayBase<T,Range> MAB;

	//ConstSlice(const Range& range);
	ConstSlice(const Range& range,
		   const Name& ownNm,
		   const MultiArrayBase<T,MARange>& multiArrayRef,
		   const typename MARange::IndexType& MAIdx,
		   const Name& MANm);

	ConstSlice(const Range& range,
		   const Name& ownNm,
		   const ConstMultiArrayOperationRoot<T,MARange>& mor,
		   const typename MARange::IndexType& MAIdx);
	
	ConstSlice(const Slice<T,Range,MARange>& slice);

	virtual const T& operator[](const typename Range::IndexType& i) const override;
	
	// link given Index to mMAPtr which is index of total array
	virtual auto beginIndex() const -> decltype(Range().begin()) override; 
	virtual auto endIndex() const -> decltype(Range().end()) override;

	virtual bool isSlice() const override;
	virtual bool isConst() const override;

	virtual void link(IndefinitIndexBase* iibPtr) const override;
	
    private:
	
	MultiArrayBase<T,MARange> const* mMultiArrayPtr = nullptr;
	mutable typename Range::IndexType mOwnIdx;
	mutable typename MARange::IndexType mMAIdx;
	typename Range::IndexType mBeginIndex;
	typename Range::IndexType mEndIndex;
    };
    
    
}

#include "slice.cc"

#endif
