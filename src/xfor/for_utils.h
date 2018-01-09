
#ifndef __for_utils_h__
#define __for_utils_h__

#include "ranges/rheader.h"
#include <array>
#include <tuple>

namespace MultiArrayHelper
{
    namespace {
        template <class Op>
        using to_size_t = size_t;
    }
    
    template <size_t N>
    struct XFPackNum
    {
	template <class IndexClass, class ExtTuple, class LastTuple, typename... Args>
	static auto mkPos(const IndexClass& ind, const ExtTuple& et, const LastTuple& lt, const Args&... args)
	    -> decltype(XFPackNum<N-1>::mkPos(et, lt, 0ul, args...))
	{
	    return XFPackNum<N-1>::mkPos(et, lt, std::get<N>(lt) + ind.pos() * std::get<N>(et), args...);
	}
    };

    template <>
    struct XFPackNum<0>
    {
	template <class IndexClass, class ExtTuple, class LastTuple, typename... Args>
	static auto mkPos(const IndexClass& ind, const ExtTuple& et, const LastTuple& lt, const Args&... args)
	    -> std::tuple<size_t,to_size_t<Args>...>
	{
	    return std::tuple<size_t,to_size_t<Args>...>(std::get<0>(lt) + ind.pos() * std::get<0>(et), args...);
	}

    };
    
} // end namespace MultiArrayHelper

#endif
