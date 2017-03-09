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

    template <typename T, class Range>
    template <class NameType>
    MultiArrayOperationRoot<T,Range> MultiArrayBase<T,Range>::operator()(const NameType& name, bool master)
    {
	if(master){
	    return MultiArrayOperationRoot<T,Range>(*this, name);
	}
	else {
	    return operator()(name);
	}
    }

    template <typename T, class Range>
    bool MultiArrayBase<T,Range>::isInit() const
    {
	return mInit;
    }
    
    /*******************
     *  MultiArray     *	     
     *******************/

    template <typename T, class Range>
    MultiArray<T,Range>::MultiArray(const Range& range) :
	MultiArrayBase<T,Range>(range),
	mCont(MAB::mRange->size())
    {
	MAB::mInit = true;
    }

    template <typename T, class Range>
    MultiArray<T,Range>::MultiArray(const Range& range, const std::vector<T>& vec) :
	MultiArrayBase<T,Range>(range),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }
    
    template <typename T, class Range>
    MultiArray<T,Range>::MultiArray(const Range& range, std::vector<T>&& vec) :
	MultiArrayBase<T,Range>(range),
	mCont(vec)
    {
	MAB::mInit = true;
	if(mCont.size() > MAB::mRange->size()){
	    mCont.erase(mCont.begin() + MAB::mRange->size(), mCont.end());
	}
    }

    template <typename T, class Range>
    template <class Range2, class Range3>
    MultiArray<T,Range>::MultiArray(const MultiArray<MultiArray<T,Range2>,Range3> in) :
	MultiArrayBase<T,Range>(merge(in.range(), in[ in.begin() ].range()))
	// assert that Range2 has always same extension
    {
	MAB::mInit = true;
	mCont.clear();
	for(auto i = in.begin(); i != in.end(); ++i){
	    mCont.insert(mCont.end(), in[i].mCont.begin(), in[i].mCont.end());
	}
	assert(mCont.size() == MAB::mRange->size());
    }

    template <typename T, class Range>
    template <class Range2, class Range3>
    MultiArray<T,Range>& MultiArray<T,Range>::operator=(const MultiArray<MultiArray<T,Range2>,Range3> in)
    {
	MAB::mRange.reset(new Range(merge(in.range(), in[ in.begin() ].range())));
	// assert that Range2 has always same extension
	mCont.clear();
	for(auto i = in.begin(); i != in.end(); ++i){
	    mCont.insert(mCont.end(), in[i].mCont.begin(), in[i].mCont.end());
	}
	assert(mCont.size() == MAB::mRange->size());
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
    
    /*
    template <typename T, class Range>
    void MultiArray<T,Range>::manipulate(ManipulatorBase<T>& mb,
					 const typename Range::IndexType& manBegin,
					 const typename Range::IndexType& manEnd)
    {
	mb.setup(mCont, manBegin.pos(), manEnd.pos());
	mb.execute();
    }
    */
}
