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
	      MutableMultiArrayBase<T,MARange>& multiArrayRef,
	      const Name& ownNm,
	      const typename MARange::IndexType& MAIdx,
	      const Name& MANm);
		
	virtual T& operator[](const typename Range::IndexType& i) override;
	virtual const T& operator[](const typename Range::IndexType& i) const override;
	
	//Slice& setSlicePos(const Index& slicePos);
	
	// link given Index to mMAPtr which is index of total array
	virtual auto begin() const -> decltype(Range().begin()) override; 
	virtual auto end() const -> decltype(Range().end()) override;

	virtual bool isSlice() const override;
	virtual bool isConst() const override;

	/*
	void set();
	*/

	/*
	void setConst(const MultiArrayBase<T,MARange>& multiArrayRef,
		      const Name& ownNm,
		      const typename MARange::IndexType& MAIdx,
		      const Name& MANm); 
	*/
	template <typename U, class RangeX, class MARangeX>	
	friend class ConstSlice;
	
    private:

	Name mOwnName;
	Name mMAName;
	MutableMultiArrayBase<T,MARange>* mMultiArrayPtr = nullptr;
	mutable typename Range::IndexType mOwnIdx;
	mutable typename MARange::IndexType mMAIdx;
	
    };

    template <typename T, class Range, class MARange/*, class Index*/>
    class ConstSlice : public MultiArrayBase<T,Range> // yes, 'Range' is correct !!!
    {
    public:

	typedef MultiArrayBase<T,Range> MAB;

	//ConstSlice(const Range& range);
	ConstSlice(const Range& range,
		   const MultiArrayBase<T,MARange>& multiArrayRef,
		   const Name& ownNm,
		   const typename MARange::IndexType& MAIdx,
		   const Name& MANm);
	ConstSlice(const Slice<T,Range,MARange>& slice);

	virtual T& operator[](const typename Range::IndexType& i) override;
	virtual const T& operator[](const typename Range::IndexType& i) const override;
	
	//Slice& setSlicePos(const Index& slicePos);
	
	// link given Index to mMAPtr which is index of total array
	virtual auto begin() const -> decltype(Range().begin()) override; 
	virtual auto end() const -> decltype(Range().end()) override;

	virtual bool isSlice() const override;
	virtual bool isConst() const override;
	
	/*
	void set(const MultiArrayBase<T,MARange>& multiArrayRef,
		 const Name& ownNm,
		 const typename MARange::IndexType& MAIdx,
		 const Name& MANm) const; 
	*/
    private:

	// !!!
	T x = static_cast<T>(0);
	
	MultiArrayBase<T,MARange> const* mMultiArrayPtr = nullptr;
	mutable typename Range::IndexType mOwnIdx;
	mutable typename MARange::IndexType mMAIdx;
    };
    
    
}

#include "slice.cc"

#endif
