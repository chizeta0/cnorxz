
#ifndef __index_for_utils_h__
#define __index_for_utils_h__

#include "ranges/rheader.h"
#include "xfor.h"
#include <array>
#include <tuple>

namespace MultiArrayHelper
{
    namespace {
        template <class Op>
        using to_size_t<Op> = size_t;
    }
    
    template <size_t N>
    struct XFPackNum
    {
	template <class Expr, class IndexClass, typename... Args>
	static auto mkFor(const std::shared_ptr<IndexClass>& iPtr, const Args&... args)
	    -> For<IndexClass,decltype(XFPackNum<N-1>::mkFor<Expr>(args...))>
	{
	    return For<IndexClass,decltype(XFPackNum<N-1>::mkFor<Expr>(args...))>(iPtr, args...);
	}

	template <class Expr, class IndexTuple, typename... Args>
	static auto mkForTp(const IndexTuple& iTuple, const Args&... args)
	    -> decltype(XFPackNum<N-1>::mkForTp(std::get<N>(iTuple), args...))
	{
	    return XFPackNum<N-1>::mkForTp(iTuple, std::get<N>(iTuple), args...)
	}
    };

    template <>
    struct XFPackNum<0>
    {
	template <class Expr, class IndexClass, typename... Args>
	static auto mkFor(const std::shared_ptr<IndexClass>& iPtr, const Args&... args)
	    -> For<IndexClass,Expr>
	{
	    return For<IndexClass,Expr>(iPtr, args...);
	}

	template <class Expr, class IndexTuple, typename... Args>
	static auto mkForTp(const IndexTuple& iTuple, const Args&... args)
	    -> decltype(XFPackNum<std::tuple_size<IndexTuple>::value-1>::mkFor( std::get<0>(iTuple), args... ))
	{
	    return XFPackNum<std::tuple_size<IndexTuple>::value-1>::mkFor( std::get<0>(iTuple), args... );
	}
    };
    
} // end namespace MultiArrayHelper

#endif
