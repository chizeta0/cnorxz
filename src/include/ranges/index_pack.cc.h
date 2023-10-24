
#ifndef __cxz_index_pack_cc_h__
#define __cxz_index_pack_cc_h__

#include "index_pack.h"

namespace CNORXZ
{
    /*************
     *   SPack   *
     *************/

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
	return SPack<Indices...,Index>( std::tuple_cat(mIs,std::make_tuple(i)) );
    }

    template <class... Indices>
    template <class Index>
    constexpr decltype(auto) SPack<Indices...>::lmul(const Sptr<Index>& i) const
    {
	return SPack<Index,Indices...>( std::tuple_cat(std::make_tuple(i), mIs) );
    }

    template <class... Indices>
    template <class... Indices2>
    constexpr decltype(auto) SPack<Indices...>::mul(const SPack<Indices2...>& p) const
    {
	return SPack<Indices...,Indices2...>( std::tuple_cat(mIs, all()) );
    }

    template <class... Indices>
    decltype(auto) SPack<Indices...>::mkRange() const
    {
	return iter<0,sizeof...(Indices)>( [&](auto i) { return get(i)->range(); },
					   [](const auto&... e) { return mrange(e...); } );
    }

    template <class... Indices>
    SizeT SPack<Indices...>::lex() const
    {
	return iter<0,sizeof...(Indices)-1>
	    ( [&](auto i) { return get(i)->lex() * iter<i+1,sizeof...(Indices)>
		    ( [&](auto j) { return get(j)->lmax().val(); },
		      [](const auto&... m) { return ( m * ... ); } ); },
		[](const auto&... e) { return ( e + ... ); } ) +
	    get(CSizeT<sizeof...(Indices)-1>{})->lex() ;
    }

    template <class... Indices>
    SizeT SPack<Indices...>::pos() const
    {
	return lex();
    }

    /**************************
     *   SPack (non-member)   *
     **************************/

    template <class... Indices>
    constexpr decltype(auto) spack(const Indices&... inds)
    {
	static_assert((is_index<Indices>::value and ...), "got non-index type");
	return SPack<Indices...>( std::make_shared<Indices>(inds)... );
    }

    template <class... Indices>
    constexpr decltype(auto) spackp(const Sptr<Indices>&... inds)
    {
	static_assert((is_index<Indices>::value and ...), "got non-index type");
	return SPack<Indices...>( inds... );
    }

    /*************
     *   DPack   *
     *************/

    template <class... Indices>
    DPack::DPack(const SPack<Indices...>& p) :
	mIs( iter<0,sizeof...(Indices)>
	     ( [&](auto i) { return xindexPtr(p[i]); },
	       [](const auto&... e) { return Vector<XIndexPtr>( { e... } ); } )
	     )
    {}
	

    /**************************
     *   DPack (non-member)   *
     **************************/

    template <class... Indices>
    DPack dpack(const Indices&... inds)
    {
	return DPack( Vector<XIndexPtr>( { xindexPtr( std::make_shared<Indices>( inds ) )... } ) );
    }

    template <class... Indices>
    DPack dpackp(const Sptr<Indices>&... inds)
    {
	return DPack( Vector<XIndexPtr>( { xindexPtr( inds )... } ) );
    }

}

#endif
