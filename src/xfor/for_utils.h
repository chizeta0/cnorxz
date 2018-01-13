
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
	template <class ETuple, typename... Args>
	static ETuple mkPos(size_t pos, const ETuple& et, const ETuple& lt, const Args&... args)
	{
	    return std::move( XFPackNum<N-1>::mkPos(pos, et, lt, std::get<N>(lt) + pos * std::get<N>(et), args...) );
	}
    };

    template <>
    struct XFPackNum<0>
    {
	template <class ETuple, typename... Args>
	static ETuple mkPos(size_t pos, const ETuple& et, const ETuple& lt, const Args&... args)
	{
	    return ETuple(std::get<0>(lt) + pos * std::get<0>(et), args...);
	}

    };
    
} // end namespace MultiArrayHelper

#endif
