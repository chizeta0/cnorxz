
#ifndef __cxz_index_pack_cc_h__
#define __cxz_index_pack_cc_h__

#include "index_pack.h"

namespace CNORXZ
{
    template <class... Indices>
    constexpr SPack<Indices...>::SPack(const Sptr<Indices>&... is) :
	mIs(is...)
    {}
    
    template <class... Indices>
    constexpr SPack<Indices...>::SPack(const Tuple<Sptr<Indices>...>& is) :
	mIs(is)
    {}

    template <class... Indices>
    constexpr const Tuple<Sptr<Indices>...>& SPack<Indices...>::all() const
    {
	return mIs;
    }
    
    template <class... Indices>
    template <SizeT I>
    constexpr decltype(auto) SPack<Indices...>::get(CSizeT<I> i) const
    {
	return std::get<I>(mIs);
    }
    
    template <class... Indices>
    template <SizeT I>
    constexpr decltype(auto) SPack<Indices...>::operator[](CSizeT<I> i) const
    {
	return get(i);
    }

    template <class... Indices>
    template <class Index>
    constexpr decltype(auto) SPack<Indices...>::rmul(const Sptr<Index>& i) const
    {
	return SPack<Indices...,Index>( std::tuple_cat(mIs,i) );
    }

    template <class... Indices>
    template <class Index>
    constexpr decltype(auto) SPack<Indices...>::lmul(const Sptr<Index>& i) const
    {
	return SPack<Index,Indices...>( std::tuple_cat(i, mIs) );
    }

    template <class... Indices>
    template <class... Indices2>
    constexpr decltype(auto) SPack<Indices...>::mul(const SPack<Indices2...>& p) const
    {
	return SPack<Indices...,Indices2...>( std::tuple_cat(mIs, all()) );
    }
}

#endif
