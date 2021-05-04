
#ifndef __static_for_h__
#define __static_for_h__

#include <cstdlib>

namespace MultiArrayTools
{
    template <size_t BEG, size_t END, int OFF, typename Incr, typename F, typename Conc>
    inline auto sfor(Incr incr, F f, Conc conc)
    {
	constexpr auto idx = std::integral_constant<size_t, BEG>{};
	constexpr auto idxm = std::integral_constant<size_t, BEG+OFF>{};
	//static_assert(abs(idx.value - END) >= abs(incr(idx) - END),
	//		  "this turns out to be a static endless loop");
	auto tmp = f(idxm);
	if constexpr(incr(idx) == END){
	    return tmp;
	}
	else {
	    return conc(tmp, sfor<incr(idx),END,OFF>(incr,f,conc));
	}
    }

    template <size_t BEG, size_t END, int OFF, typename Incr, typename F, typename Conc, typename Arg>
    inline auto sfor(Incr incr, F f, Conc conc, const Arg& arg)
    {
	constexpr auto idx = std::integral_constant<size_t, BEG>{};
	constexpr auto idxm = std::integral_constant<size_t, BEG+OFF>{};
	//static_assert(abs(idx.value - END) >= abs(incr(idx) - END),
	//		  "this turns out to be a static endless loop");
	auto tmp = f(idxm);
	if constexpr(incr(idx) == END){
	    return conc(tmp, arg);
	}
	else {
	    return conc(tmp, sfor<incr(idx),END,OFF>(incr,f,conc,arg));
	}
    }

    template <size_t BEG, size_t END, int OFF, typename Incr, typename F, typename Create, typename... Args>
    inline auto unpack(Incr incr, F f, Create create, const Args&... args)
    {
	constexpr auto idx = std::integral_constant<size_t, BEG>{};
	constexpr auto idxm = std::integral_constant<size_t, BEG+OFF>{};
	//static_assert(abs(idx.value - END) >= abs(incr(idx) - END),
	//		  "this turns out to be a static endless loop");
	if constexpr(BEG == END){
	    return create(args...);
	}
	else {
	    auto tmp = f(idxm);
	    return unpack<incr(idx),END,OFF>(incr, f, create, args..., tmp);
	}
    }
    
}

#define MA_SFOR(i,beg,end,incr,expr) sfor<beg,end,0>([&](auto i) constexpr { return incr; }, [&](auto i){ expr return 0; }, [&](auto f, auto next) { return 0; })
#define MA_SCFOR(i,beg,end,incr,expr,conc) sfor<beg,end,0>([&](auto i) constexpr { return incr; }, [&](auto i){ return expr; }, [&](auto f, auto next) { return f.conc(next); })
#define MA_SRFOR(i,beg,end,decr,expr) sfor<beg,end,-1>([&](auto i) constexpr { return decr; }, [&](auto i){ expr return 0; }, [&](auto f, auto next) { return 0; })
#define MA_SCRFOR(i,beg,end,decr,expr,conc) sfor<beg,end,-1>([&](auto i) constexpr { return decr; }, [&](auto i){ return expr; }, [&](auto f, auto next) { return f.conc(next); })
#define MA_SCRAFOR(i,beg,end,decr,expr,conc,arg) sfor<beg,end,-1>([&](auto i) constexpr { return decr; }, [&](auto i){ return expr; }, [&](auto f, auto next) { return f.conc(next); }, arg)
#define MA_CFOR(i,beg,end,incr,expr,cre) unpack<beg,end,0>([&](auto i) constexpr { return incr; }, [&](auto i){ expr }, [&](auto... args) { return cre(args...); })

#define MA_SCFOR_X(i,beg,end,incr,expr,conc) sfor<beg,end,0>([](auto i) constexpr { return incr; }, [](auto i){ return expr; }, [](auto f, auto next) { return f.conc(next); })

#endif
