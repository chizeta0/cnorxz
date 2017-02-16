
#include "multi_array.h"

namespace MultiArrayTools
{
    /*******************
     *  MultiArray     *	     
     *******************/

    template <typename... Ranges>
    void giveNames(const std::string& name, /**/);

    template <typename... Ranges>
    void giveNames(const std::vector<std::string>& names, /**/);

    /*!!!!  giveNames(...)  !!!!!*/
    
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
	// give names... !!!
	return MultiArrayOperation<T,Range>(*this, index);
    }

}
