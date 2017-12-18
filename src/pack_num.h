// -*- C++ -*-

#ifndef __pack_num_h__
#define __pack_num_h__

#include <cstdlib>
//#include <type_traits>
#include <tuple>
#include <ostream>

#include "base_def.h"

namespace MultiArrayHelper
{

    template <size_t N>
    struct PackNum
    {
	
	template <class... Ops>
	static void makeBlockTypeVec(std::vector<std::pair<BlockType,size_t> >& btv,
				     const std::tuple<Ops...>& ops,
				     std::shared_ptr<VIWB> idxPtr)
	{
	    auto subvec = std::move( std::get<N>(ops).block(idxPtr) );
	    btv.insert(btv.end(), subvec.begin(), subvec.end() );
	    PackNum<N-1>::makeBlockTypeVec(btv, ops, idxPtr);
	}

	template <typename T, class Func, class ArgTuple, class... Args>
	static BlockResult<T> unpackArgs(const ArgTuple& tp, const Args&... args)
	{
	    return PackNum<N-1>::template unpackArgs<T,Func>(tp, std::get<N>(tp).get(), args...);
	}

	template <typename... T>
	static void printTuple(std::ostream& out, const std::tuple<T...>& tp){
	    out << std::get<sizeof...(T)-N-1>(tp) << ", ";
	    PackNum<N-1>::printTuple(out, tp);
	}

    };
    
    template<>
    struct PackNum<0>
    {

	template <typename T, class Func, class ArgTuple, class... Args>
	static BlockResult<T> unpackArgs(const ArgTuple& tp, const Args&... args)
	{
	    static_assert(sizeof...(Args) == std::tuple_size<ArgTuple>::value-1,
			  "inconsistent number of arguments");
	    static BlockBinaryOp<T,Func,decltype(std::get<0>(tp).get()), decltype(args)...> f;
	    return f(std::get<0>(tp).get(), args...);
	}
	
	template <class... Ops>
	static void makeBlockTypeVec(std::vector<std::pair<BlockType,size_t> >& btv,
				     const std::tuple<Ops...>& ops,
				     std::shared_ptr<VIWB> idxPtr)
	{
	    auto subvec = std::move( std::get<0>(ops).block(idxPtr) );
	    btv.insert(btv.end(), subvec.begin(), subvec.end() );
	}
	
	template <typename... T>
	static void printTuple(std::ostream& out, const std::tuple<T...>& tp){
	    out << std::get<sizeof...(T)-1>(tp);
	}
	
    };


    
} // end namespace MultiArrayHelper

#endif
