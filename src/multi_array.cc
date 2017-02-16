
#include "multi_array.h"

namespace MultiArrayTools
{
    /*******************
     *  MultiArray     *	     
     *******************/

    namespace
    {
	template <typename... Ranges>
	void giveNames(const std::string& name, /**/);
	
	template <typename... Ranges>
	void giveNames(const std::vector<std::string>& names, /**/);

    }
    /*!!!!  giveNames(...)  !!!!!*/

    template <typename T, class Range>
    MultiArray(const Range& range) : mRange(&range), mCont(mRange.size())
    {
	mInit = true;
    }

    template <typename T, class Range>
    MultiArray(const Range& range, const std::vector<T>& vec) : mRange(&range), mCont(vec)
    {
	mInit = true;
	if(mCont.size() > mRange.size()){
	    mCont.erase(mCont.begin() + mRange.size(), mCont.end());
	}
    }
    
    template <typename T, class Range>
    MultiArray(const Range& range, std::vector<T>&& vec) : mRange(&range), mCont(vec)
    {
	mInit = true;
	if(mCont.size() > mRange.size()){
	    mCont.erase(mCont.begin() + mRange.size(), mCont.end());
	}
    }
    
    template <typename T, class Range>
    T& MultiArray<T,Is...>::operator()(const typename Range::indexType& i)
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class Range>
    const T& MultiArray<T,Is...>::operator()(const typename Range::indexType& i) const
    {
	return mCont[ i.pos() ];
    }

    template <typename T, class Range>
    template <class... NameTypes>
    MultiArrayOperation<T,Range>& operator()(const NameTypes&... str) const
    {
	auto index = mRange->begin();
	index.name(Name("master", str...));
	return MultiArrayOperation<T,Range>(*this, index);
    }

}
