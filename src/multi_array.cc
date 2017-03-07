// -*- C++ -*-

#include "multi_array.h"

namespace MultiArrayTools
{
    /**********************
     *  MultiArrayBase    *	     
     **********************/

    template <typename T, class Range>
    MultiArrayBase<T,Range>::MultiArrayBase(const Range& range) : mRange(new Range(range)) {}
    
    template <typename T, class Range>
    size_t MultiArrayBase<T,Range>::size() const
    {
	return mRange->size();
    }
    
    template <typename T, class Range>
    auto MultiArrayBase<T,Range>::begin() -> decltype(Range().begin())
    {
	return mRange->begin();
    }

    template <typename T, class Range>
    auto MultiArrayBase<T,Range>::end() -> decltype(Range().end())
    {
	return mRange->end();
    }

    template <typename T, class Range>
    const Range& MultiArrayBase<T,Range>::range() const
    {
	return *mRange;
    }

    template <typename T, class Range>
    template <class... NameTypes>
    MultiArrayOperationRoot<T,Range> MultiArrayBase<T,Range>::operator()(const NameTypes&... str)
    {
	return MultiArrayOperationRoot<T,Range>(*this, Name("master", str...));
    }
    
    /*******************
     *  MultiArray     *	     
     *******************/

    template <typename T, class Range>
    MultiArray<T,Range>::MultiArray(const Range& range) :
	MultiArrayBase<T,Range>(range),
	mCont(MAB::mRange->size()) {}

    template <typename T, class Range>
    MultiArray<T,Range>::MultiArray(const Range& range, const std::vector<T>& vec) :
	MultiArrayBase<T,Range>(range),
	mCont(vec)
    {
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }
    
    template <typename T, class Range>
    MultiArray<T,Range>::MultiArray(const Range& range, std::vector<T>&& vec) :
	MultiArrayBase<T,Range>(range),
	mCont(vec)
    {
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }

    template <typename T, class Range>
    T& MultiArray<T,Range>::operator[](const typename Range::IndexType& i)
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class Range>
    const T& MultiArray<T,Range>::operator[](const typename Range::IndexType& i) const
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class Range>
    bool MultiArray<T,Range>::isSlice() const
    {
	return false;
    }

    template <typename T, class Range>
    void MultiArray<T,Range>::manipulate(ManipulatorBase<T>& mb, size_t manBegin, size_t manEnd)
    {
	//mb.reset();
	mb.setup(mCont, manBegin, manEnd);
	mb.execute();
    }
    
}
