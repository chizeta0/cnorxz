// -*- C++ -*-

#include "me.h"

namespace ME
{

    /**************
     *  IndexBase *	     
     **************/
    
    size_t IndexBase::size() const
    {
	return mPoss->size();
    }

    IndexBase& IndexBase::operator=(size_t pos)
    {
	mPos = pos;
    }


    /********************
     *  SingleIndexBase *	     
     ********************/

    
    template <typename U, IndexType TYPE>
    SingleIndexBase& SingleIndexBase<U,TYPE>::operator=(size_t pos)
    {
	mPos = pos;
	mActPos = (*mPoss)[pos];
    }

    template <typename U, IndexType TYPE>
    SingleIndexBase& SingleIndexBase<U,TYPE>::operator=(const U& upos)
    {
	size_t cnt = 0;
	for(auto& x: *mPoss){
	    if(x == upos){
		mPos = cnt;
		return *this;
	    }
	    ++cnt;
	}
	// THROW !!
	return *this;
    }

    template <typename U, IndexType TYPE>
    const U& SingleIndexBase<U,TYPE>::getActPos() const
    {
	return mActPos;
    }
    

    /*******************
     *  MultiIndexBase *	     
     *******************/

    namespace
    {

	template <class Tuple, size_t N>
	size_t size(const Tuple& tp, size_t curSize = 1)
	{
	    return size<Tuple,N-1>( tp, std::get<N>(tp).size() * curSize );
	}
	
	template <class Tuple>
	size_t size<Tuple,0>(const Tuple& tp, size_t curSize = 1)
	{
	    return std::get<0>(tp).size() * curSize;
	}

	template <class Tuple, size_t N>
	size_t pos(const Tuple& tp)
	{
	    const size_t M = std::tuple_size(tp) - N;
	    return pos<Tuple,M-1>(tp) * std::get<M-1>(tp).size() + std::get<M>(tp).pos()
	}

	template <class Tuple>
	size_t pos<Tuple,0>(const Tuple& tp)
	{
	    const size_t M = std::tuple_size(tp);
	    return std::get<M>(tp).pos();
	}

	size_t varpos(size_t curPos, const I& i)
	{
	    return curPos * i.size() + i.pos();
	}
	
	template <class... Is>
	size_t varpos(size_t curPos, const I& i, const Is&... is)
	{
	    return varpos(curPos * i.size() + i.pos(), is...);
	}
    }
    
    template <class... Is>
    size_t MultiIndexBase<Is...>::size() const
    {
	return size<ContType, indNum>(mCont);
    }

    template <class... Is>
    MultiIndexBase& MultiIndexBase<Is...>::operator=(const ContType& upos)
    {
	mCont = upos;
	mPos = pos<ContType, indNum>(mCont);
    }

    const ContType& MultiIndexBase<Is...>::getActPos() const
    {
	return mCont;
    }

    template <size_t N>
    auto together() -> decltype(std::tuple_cat(make_left<ContType,N>(mCont),
					       make_right<ContType,N+1>(mCont) ))
    {
	return std::tuple_cat(make_left<ContType,N-1>(mCont), make_right<ContType,N+1>(mCont));
    }

    template <class I, size_t N>
    auto add(const I& in) -> decltype(std::tuple_cat(make_left<ContType,N>(mCont), in,
						     make_right<ContType,N+1>(mCont)))
    {
	return std::tuple_cat(make_left<ContType,N>(mCont), in, make_right<ContType,N+1>(mCont));
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

