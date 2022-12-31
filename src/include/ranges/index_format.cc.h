
#ifndef __cxz_index_format_cc_h__
#define __cxz_index_format_cc_h__

#include "index_format.h"

namespace CNORXZ
{
    /***************
     *   MFormat   *
     ***************/
    
    template <SizeT N>
    constexpr MFormat<N>::MFormat(const Arr<UPos,N>& b) :
	mB(b)
    {}

    template <SizeT N>
    template <class FormatT>
    constexpr MFormat<N>::MFormat(const FormatT& f)
    {
	static_assert(f.size() == N, "try to assign format of wrong dimension");
	iter<0,N>( [&](auto i) { mB[i] = f[i]; }, NoF{} );
    }

    template <SizeT N>
    const Arr<UPos,N>& MFormat<N>::all() const
    {
	return mB;
    }

    template <SizeT N>
    constexpr decltype(auto) MFormat<N>::size() const
    {
	return std::integral_constant<SizeT,N>{};
    }

    template <SizeT N>
    template <SizeT I>
    constexpr decltype(auto) MFormat<N>::get(std::integral_constant<SizeT,I> i) const
    {
	return std::get<I>(mB);
    }

    template <SizeT N>
    template <SizeT I>
    constexpr decltype(auto) MFormat<N>::operator[](std::integral_constant<SizeT,I> i) const
    {
	return get(i);
    }

    
    /****************
     *   GMFormat   *
     ****************/

    template <class... PosT>
    constexpr GMFormat<PosT...>::GMFormat(const Tuple<PosT...>& b) :
	mB(b)
    {}

    template <class... PosT>
    template <class FormatT>
    constexpr GMFormat<PosT...>::GMFormat(const FormatT& f)
    {
	static_assert(f.size() == size(), "try to assign format of wrong dimension");
	iter<0,sizeof...(PosT)>( [&](auto i) { mB[i] = f[i]; }, NoF{} );
    }

    template <class... PosT>
    const Tuple<PosT...>& GMFormat<PosT...>::all() const
    {
	return mB;
    }

    template <class... PosT>
    constexpr decltype(auto) GMFormat<PosT...>::size() const
    {
	return std::integral_constant<SizeT,sizeof...(PosT)>{};
    }

    template <class... PosT>
    template <SizeT I>
    constexpr decltype(auto) GMFormat<PosT...>::get(std::integral_constant<SizeT,I> i) const
    {
	return std::get<I>(mB);
    }

    template <class... PosT>
    template <SizeT I>
    constexpr decltype(auto) GMFormat<PosT...>::operator[](std::integral_constant<SizeT,I> i) const
    {
	return get(i);
    }

    /***************
     *   YFormat   *
     ***************/

    template <class FormatT>
    YFormat::YFormat(const FormatT& f) : mB(f.size())
    {
	// replace by check if integral constant
	if constexpr(is_static_format<FormatT>::value){
	    iter<0,f.size()>( [&](auto i) { mB[i] = f[i]; }, NoF{} );
	}
	else {
	    for(SizeT i = 0; i != mB.size(); ++i){
		mB[i] = f[i];
	    }
	}
    }

    template <SizeT I>
    const UPos& YFormat::get(std::integral_constant<SizeT,I> i) const
    {
	return mB[i];
    }
	
    template <SizeT I>
    const UPos& YFormat::operator[](std::integral_constant<SizeT,I> i) const
    {
	return mB[i];
    }

}

#endif
