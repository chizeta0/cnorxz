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
				     std::shared_ptr<VIWB> idxPtr, bool init)
	{
	    auto subvec = std::move( std::get<N>(ops).block(idxPtr, init) );
	    btv.insert(btv.end(), subvec.begin(), subvec.end() );
	    PackNum<N-1>::makeBlockTypeVec(btv, ops, idxPtr, init);
	}

	template <class... Ops>
	static void makeBlockTypeVec(std::vector<std::pair<BlockType,size_t> >& btv,
				     const std::tuple<Ops...>& ops,
				     const IndexInfo* idxPtr, bool init)
	{
	    auto subvec = std::move( std::get<N>(ops).block(idxPtr, init) );
	    btv.insert(btv.end(), subvec.begin(), subvec.end() );
	    PackNum<N-1>::makeBlockTypeVec(btv, ops, idxPtr, init);
	}
	
	template <typename T, class Func, class ArgTuple, class... Args>
	static void unpackArgs(BlockResult<T>& res, const ArgTuple& tp, const Args&... args)
	{
	    PackNum<N-1>::template unpackArgs<T,Func>(res, tp, std::get<N>(tp).get(), args...);
	}

	template <typename... T>
	static void printTuple(std::ostream& out, const std::tuple<T...>& tp)
	{
	    out << std::get<sizeof...(T)-N-1>(tp) << ", ";
	    PackNum<N-1>::printTuple(out, tp);
	}

	template <class... Ops>
	static auto mkStepTuple(std::intptr_t ii, std::tuple<Ops const&...> otp)
	    -> decltype(std::tuple_cat( PackNum<N-1>::mkStepTuple(ii, otp), std::get<N>(otp).rootSteps(ii) ))
	{
	    return std::tuple_cat( PackNum<N-1>::mkStepTuple(ii, otp), std::get<N>(otp).rootSteps(ii) );
	}
	
	template <class RootStepTuple, class IndexClass, class OpClass>
	static void mkExt(std::array<RootStepTuple,IndexClass::totalDim()>& out,
			  const std::array<std::intptr_t,IndexClass::totalDim()>& siar,
			  const OpClass& second)
	{
	    std::get<N>(out) = second.rootSteps( std::get<N>(siar) );
	    PackNum<N-1>::mkExt(out, siar, second);
	}

	// call with -2 (instead of -1)
	template <typename T, class ETuple, class OpTuple, class OpFunction, size_t START>
	static T&& mkOpExpr(const ETuple& pos, const OpTuple& ops)
	{
	    static const size_t NEXT = START - std::tuple_element<N+1,OpTuple>::type::SIZE;
	    return std::forward<T>
		( OpFunction::apply( std::get<N+1>(ops).template get<START>(pos),
				     PackNum<N-1>::template mkOpExpr<ETuple,OpTuple,OpFunction,NEXT>(pos, ops) ) );
	}

	template <class OpTuple, class Expr>
	static auto mkLoop( const OpTuple& ot, Expr&& exp )
	    -> decltype(std::get<N>(ot).loop( PackNum<N-1>::mkLoop(ot,exp) ))&&
	{
	    return std::get<N>(ot).loop( PackNum<N-1>::mkLoop(ot,exp) );
	}
    };
    
    template<>
    struct PackNum<0>
    {

	template <typename T, class Func, class ArgTuple, class... Args>
	static void unpackArgs(BlockResult<T>& res, const ArgTuple& tp, const Args&... args)
	{
	    static_assert(sizeof...(Args) == std::tuple_size<ArgTuple>::value-1,
			  "inconsistent number of arguments");
	    BlockBinaryOp<T,Func,decltype(std::get<0>(tp).get()), decltype(args)...> f(res);
	    f(std::get<0>(tp).get(), args...);
	}
	
	template <class... Ops>
	static void makeBlockTypeVec(std::vector<std::pair<BlockType,size_t> >& btv,
				     const std::tuple<Ops...>& ops,
				     std::shared_ptr<VIWB> idxPtr, bool init)
	{
	    auto subvec = std::move( std::get<0>(ops).block(idxPtr, init) );
	    btv.insert(btv.end(), subvec.begin(), subvec.end() );
	}

	template <class... Ops>
	static void makeBlockTypeVec(std::vector<std::pair<BlockType,size_t> >& btv,
				     const std::tuple<Ops...>& ops,
				     const IndexInfo* idxPtr, bool init)
	{
	    auto subvec = std::move( std::get<0>(ops).block(idxPtr, init) );
	    btv.insert(btv.end(), subvec.begin(), subvec.end() );
	}

	template <typename... T>
	static void printTuple(std::ostream& out, const std::tuple<T...>& tp)
	{	    
	    out << std::get<sizeof...(T)-1>(tp);
	}

	template <class... Ops>
	static auto mkStepTuple(std::intptr_t ii, std::tuple<Ops const&...> otp)
	    -> decltype(std::get<0>(otp).rootSteps(ii))
	{
	    return std::get<0>(otp).rootSteps(ii);
	}

	template <class RootStepTuple, class IndexClass, class OpClass>
	static void mkExt(std::array<RootStepTuple,IndexClass::totalDim()>& out,
			  const std::array<std::intptr_t,IndexClass::totalDim()>& siar,
			  const OpClass& second)
	{
	    std::get<0>(out) = second.rootSteps( std::get<0>(siar) );
	}

	template <typename T, class ETuple, class OpTuple, class OpFunction, size_t START>
	static T&& mkOpExpr(const ETuple& pos, const OpTuple& ops)
	{
	    static const size_t NEXT = START - std::tuple_element<1,OpTuple>::type::SIZE;
	    return std::forward<T>
		( OpFunction::apply( std::get<1>(ops).template get<START>(pos),
				     std::get<0>(ops).template get<NEXT>(pos) ) );
	}

	template <class OpTuple, class Expr>
	static auto mkLoop( const OpTuple& ot, Expr&& exp )
	    -> decltype(std::get<0>(ot).loop( exp ))&&
	{
	    return std::get<0>(ot).loop( exp );
	}

	
    };


    
} // end namespace MultiArrayHelper

#endif
