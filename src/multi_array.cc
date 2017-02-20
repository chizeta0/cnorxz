
#include "multi_array.h"

namespace MultiArrayTools
{
    /*******************
     *  MultiArray     *	     
     *******************/

    template <typename T, class Range>
    MultiArray<T,Range>::MultiArray(const Range& range) : mRange(new Range(range)), mCont(mRange.size())
    {
	mInit = true;
    }

    template <typename T, class Range>
    MultiArray<T,Range>::MultiArray(const Range& range, const std::vector<T>& vec) : mRange(new Range(range)),
										     mCont(vec)
    {
	mInit = true;
	if(mCont.size() > mRange->size()){
	    mCont.erase(mCont.begin() + mRange->size(), mCont.end());
	}
    }
    
    template <typename T, class Range>
    MultiArray<T,Range>::MultiArray(const Range& range, std::vector<T>&& vec) : mRange(new Range(range)), mCont(vec)
    {
	mInit = true;
	if(mCont.size() > mRange->size()){
	    mCont.erase(mCont.begin() + mRange->size(), mCont.end());
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
    template <class... NameTypes>
    MultiArrayOperationBase<T,Range> MultiArray<T,Range>::operator()(const NameTypes&... str)
    {
	auto index = mRange->begin();
	index.name(Name("master", str...));
	return MultiArrayOperationBase<T,Range>(*this, index);
    }

    template <typename T, class Range>
    size_t MultiArray<T,Range>::size() const
    {
	return mRange->size();
    }

    template <typename T, class Range>
    auto MultiArray<T,Range>::begin() -> decltype(Range().begin())
    {
	return mRange->begin();
    }

    template <typename T, class Range>
    auto MultiArray<T,Range>::end() -> decltype(Range().end())
    {
	return mRange->end();
    }

    
}
