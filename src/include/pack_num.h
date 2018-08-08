// -*- C++ -*-

#ifndef __pack_num_h__
#define __pack_num_h__

#include <cstdlib>
//#include <type_traits>
#include <tuple>
#include <ostream>

#include "base_def.h"
#include "xfor/exttype.h"

namespace MultiArrayHelper
{
    template <bool ISSTATIC>
    struct Application
    {
	template <class OpFunction, typename... Ts>
	static inline auto apply(std::shared_ptr<OpFunction> f, Ts... as)
	    -> decltype((*f)(as...))
	{
	    return (*f)(as...);
	}
    };

    template <>
    struct Application<true>
    {
	template <class OpFunction, typename... Ts>
	static inline auto apply(std::shared_ptr<OpFunction> f, Ts... as)
	    -> decltype(OpFunction::apply(as...))
	{
	    return OpFunction::apply(as...);
	}
    };
    
    
    template <size_t N>
    struct PackNum
    {
	template <class MA, class ITuple, class... IPtrs>
	static auto mkElemOperation(const MA& ma, const ITuple& ituple, IPtrs... iptrs)
	    -> decltype(PackNum<N-1>::mkElemOperation(ma, ituple, std::get<N>(ituple), iptrs...))
	{
	    return PackNum<N-1>::mkElemOperation(ma, ituple, std::get<N>(ituple), iptrs...);
	}

	
	template <typename... T>
	static void printTuple(std::ostream& out, const std::tuple<T...>& tp)
	{
	    out << std::get<sizeof...(T)-N-1>(tp) << ", ";
	    PackNum<N-1>::printTuple(out, tp);
	}

	template <class... Ops>
	static auto mkSteps(std::intptr_t ii, const std::tuple<Ops...>& otp)
	    -> decltype(PackNum<N-1>::mkSteps(ii, otp).extend( std::get<N>(otp).rootSteps(ii)) )
	{
	    return PackNum<N-1>::mkSteps(ii, otp).extend( std::get<N>(otp).rootSteps(ii));
	}
	
	template <class RootStepTuple, class IndexClass, class OpClass>
	static void mkExt(std::array<RootStepTuple,IndexClass::totalDim()>& out,
			  const std::array<std::intptr_t,IndexClass::totalDim()>& siar,
			  const OpClass& second)
	{
	    std::get<N>(out) = second.rootSteps( std::get<N>(siar) );
	    PackNum<N-1>::mkExt(out, siar, second);
	}

	template <size_t LAST, typename T, class ETuple, class OpTuple, class OpFunction, typename... Args>
	static inline T mkOpExpr(std::shared_ptr<OpFunction> f, const ETuple& pos, const OpTuple& ops, const Args&... args)
	{
	    typedef typename std::remove_reference<decltype(std::get<N>(ops))>::type NextOpType;
	    static_assert(LAST > NextOpType::SIZE, "inconsistent array positions");
	    static constexpr size_t NEXT = LAST - NextOpType::SIZE;
	    typedef decltype(std::get<N>(ops).get(Getter<NEXT>::template getX<ETuple>( pos ))) ArgT;
	    return PackNum<N-1>::template mkOpExpr<NEXT,T,ETuple,OpTuple,OpFunction,ArgT,Args...>
		( f, pos, ops, std::get<N>(ops).get(Getter<NEXT>::template getX<ETuple>( pos )), args...);
	}

	template <class OpTuple, class Expr>
	static auto mkLoop( const OpTuple& ot, Expr exp )
	    -> decltype(std::get<N>(ot).loop( PackNum<N-1>::mkLoop(ot,exp) ))
	{
	    return std::get<N>(ot).loop( PackNum<N-1>::mkLoop(ot,exp) );
	}

	template <typename T, class Op, class... SRanges>
	static void mkSliceBlocks(std::array<size_t, sizeof...(SRanges)+1>& blocks,
				  const ContainerIndex<T,typename SRanges::IndexType...>& index,
				  const Op& op, size_t total = 1)
	{
	    const size_t tmp = 
		op.rootSteps(reinterpret_cast<std::intptr_t>
			     ( index.template getPtr<N>().get() ) )
		.val();
	    std::get<N+1>(blocks) = tmp;
	    PackNum<N-1>::template mkSliceBlocks<T,Op,SRanges...>(blocks, index, op, total * tmp);
	}

	template <class... SRanges>
	static bool checkIfSameInstance(const std::tuple<std::shared_ptr<SRanges>...>& rtp1,
					const std::tuple<std::shared_ptr<SRanges>...>& rtp2)
	{
	    return std::get<N>(rtp1).get() == std::get<N>(rtp2).get() and PackNum<N-1>::checkIfSameInstance(rtp1,rtp2);
	}

	template <class MA, class ITuple, class... Indices>
	static inline auto mkMapOp(const MA& ma, const ITuple& itp, const std::shared_ptr<Indices>&... inds)
	    -> decltype(PackNum<N-1>::mkMapOp(ma, itp, std::get<N>(itp), inds...))
	{
	    return PackNum<N-1>::mkMapOp(ma, itp, std::get<N>(itp), inds...);
	}
    };
    
    template<>
    struct PackNum<0>
    {
	template <class MA, class ITuple, class... IPtrs>
	static auto mkElemOperation(const MA& ma, const ITuple& ituple, IPtrs... iptrs)
	    -> decltype(ma(iptrs...))
	{
	    return ma(iptrs...);
	}
	
	template <typename... T>
	static void printTuple(std::ostream& out, const std::tuple<T...>& tp)
	{	    
	    out << std::get<sizeof...(T)-1>(tp);
	}

	template <class... Ops>
	static auto mkSteps(std::intptr_t ii, const std::tuple<Ops...>& otp)
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
	
	template <size_t LAST, typename T, class ETuple, class OpTuple, class OpFunction, typename... Args>
	static inline T mkOpExpr(std::shared_ptr<OpFunction> f, const ETuple& pos, const OpTuple& ops, const Args&... args)
	{
	    typedef typename std::remove_reference<decltype(std::get<0>(ops))>::type NextOpType;
	    static constexpr size_t NEXT = LAST - NextOpType::SIZE;
	    static_assert(NEXT == 0, "inconsistent array positions");
	    typedef decltype(std::get<0>(ops).get(Getter<0>::template getX<ETuple>( pos ))) ArgT;
	    return Application<OpFunction::FISSTATIC>::template apply<OpFunction,ArgT,Args...>(f, std::get<0>(ops).get(Getter<0>::template getX<ETuple>( pos )), args...);
	    //return OpFunction::apply(std::get<0>(ops).get(Getter<0>::template getX<ETuple>( pos )), args...);
	}

	template <class OpTuple, class Expr>
	static auto mkLoop( const OpTuple& ot, Expr exp )
	    -> decltype(std::get<0>(ot).loop( exp ))
	{
	    return std::get<0>(ot).loop( exp );
	}

	template <typename T, class Op, class... SRanges>
	static void mkSliceBlocks(std::array<size_t, sizeof...(SRanges)+1>& blocks,
				  const ContainerIndex<T,typename SRanges::IndexType...>& index,
				  const Op& op, size_t total = 1)
	{
	    const size_t tmp = 
		op.rootSteps(reinterpret_cast<std::intptr_t>
			     ( index.template getPtr<0>().get() ) )
		.val();
	    std::get<1>(blocks) = tmp;
	    std::get<0>(blocks) = total * tmp; // this is not correct, but not used so far ... !!!
	}

	template <class... SRanges>
	static bool checkIfSameInstance(const std::tuple<std::shared_ptr<SRanges>...>& rtp1,
					const std::tuple<std::shared_ptr<SRanges>...>& rtp2)
	{
	    return std::get<0>(rtp1).get() == std::get<0>(rtp2).get();
	}

	template <class MA, class ITuple, class... Indices>
	static inline auto mkMapOp(const MA& ma, const ITuple& itp, const std::shared_ptr<Indices>&... inds)
	    -> decltype(ma.exec(std::get<0>(itp), inds...))
	{
	    return ma.exec(std::get<0>(itp), inds...);
	}

    };


    
} // end namespace MultiArrayHelper

#endif
