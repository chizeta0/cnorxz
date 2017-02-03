// -*- C++ -*-

#include "me.h"

namespace ME
{

    /******************
     *   RangeBase    *
     ******************/

    template <class Index>
    bool RangeBase<Index>::isSubRange() const
    {
	return false;
    }

    /*********************
     *   SubRangeBase    *
     *********************/

    template <class Index>
    bool SubRangeBase<Index>::isSubRange() const
    {
	return true;
    }

    /********************
     *   SingleRange    *
     ********************/

    template <typename U, RangeType TYPE>
    const U& SingleRange<U,TYPE>::get(size_t pos) const
    {
	return mSpace[pos];
    }

    template <typename U, RangeType TYPE>
    size_t SingleRange<U,TYPE>::get(const U& metaPos) const
    {
	size_t cnt = 0;
	for(auto& x: mSpace){
	    if(x == metaPos){
		return cnt;
	    }
	    ++cnt;
	}
	return cnt;
    }
    
    /**************
     *  IndexBase *	     
     **************/

    template <class Index>
    Index& IndexBase<Index>::operator=(const Index& in)
    {
	mPos = evaluate(in);
    }

    template <class Index>
    Index& IndexBase<Index>::operator=(size_t pos)
    {
	mPos = pos;
	return *this;
    }

    template <class Index>
    Index& IndexBase<Index>::operator++()
    {
	++mPos;
	return *this;
    }

    template <class Index>
    Index& IndexBase<Index>::operator--()
    {
	--mPos;
	return *this;
    }

    template <class Index>
    Index& IndexBase<Index>::operator+=(int n)
    {
	mPos += n;
	return *this;
    }

    template <class Index>
    Index& IndexBase<Index>::operator-=(int n)
    {
	mPos -= n;
	return *this;
    }

    template <class Index>
    bool IndexBase<Index>::operator==(const Index& i)
    {
	return mRange == i.mRange and mPos == i.mPos;
    }

    template <class Index>
    bool IndexBase<Index>::operator!=(const Index& i)
    {
	return mRange != i.mRange or mPos != i.mPos;
    }

    /********************
     *  SingleIndexBase *	     
     ********************/
    
    template <typename U, IndexType TYPE>
    const U& SingleIndexBase<U,TYPE>::getMetaPos() const
    {
	return dynamic_cast<SingleRange*>( mRange )->get(mPos);
    }

    template <typename U, IndexType TYPE>
    size_t SingleIndexBase<U,TYPE>::evaluate(const Index& in)
    {
	return in.mPos;
    }
    

    /*******************
     *  MultiIndexBase *	     
     *******************/

    namespace
    {
	template <size_t N, class MultiIndex>
	size_t evaluate_x(const MultiIndex& index)
	{
	    const auto& subIndex = index.getIndex<N>();
	    return evaluate_x<N-1>(index) * subIndex.size() + subIndex.pos();
	}

	template <class MultiIndex>
	size_t evaluate_x<0>(const MultiIndex& index)
	{
	    const auto& subIndex = index.getIndex<0>();
	    return subIndex.pos();
	}
    }
            
    template <class... Indices>
    size_t MultiIndex<Indices...>::evaluate(const MultiIndex<Indices...>& in) const
    {
	return evaluate_x<sizeof...(Indices)-1>(in);
    }
    
    /*******************
     *  MultiArray     *	     
     *******************/

    template <typename T, class... Is>
    T& MultiArray<T,Is...>::operator()(const Is&... is)
    {
	return mCont[varpos(0, is...)];
    }

    template <typename T, class... Is>
    const T& MultiArray<T,Is...>::operator()(const Is&... is) const
    {
	return mCont[varpos(0, is...)];
    }
    
} // end namespace ME

