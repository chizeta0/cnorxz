
#ifndef __static_for_h__
#define __static_for_h__

#include <cstdlib>

namespace MultiArrayTools
{
    template <size_t BEG, size_t END, int OFF, typename Incr, typename F>
    inline void sfor(Incr incr, F f)
    {
	constexpr auto idx = std::integral_constant<size_t, BEG>{};
	constexpr auto idxm = std::integral_constant<size_t, BEG+OFF>{};

	const bool cond = f(idxm);
	if constexpr(incr(idx) != END){
	    if(cond){
		sfor<incr(idx),END,OFF>(incr,f);
	    }
	}
    }

    template <size_t BEG, size_t END, int OFF, typename Incr, typename F, typename Cond>
    inline auto sforx(Incr incr, F f, Cond cond)
    {
	constexpr auto idx = std::integral_constant<size_t, BEG>{};
	constexpr auto idxm = std::integral_constant<size_t, BEG+OFF>{};

	if constexpr(incr(idx) != END){
	    if(cond(idx)){
		return sforx<incr(idx),END,OFF>(incr,f,cond);
	    }
	    else {
		return f(idxm);
	    }
	}
	else {
	    return f(idxm);
	}
    }

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

#define MA_INC [&](auto i) constexpr { return i+1; }
#define MA_DEC [&](auto i) constexpr { return i-1; }
#define MA_ZCONC [&](auto a, auto b) { return 0; }

namespace MultiArrayTools
{
    template <size_t BEG, size_t END, typename F>
    inline auto sfor_p(F f)
    {
	return sfor<BEG,END,0>(MA_INC,f);
    }

    template <size_t BEG, size_t END, typename F>
    inline auto sfor_m(F f)
    {
	return sfor<BEG,END,-1>(MA_DEC,f);
    }

    template <size_t BEG, size_t END, typename F, typename Cond>
    inline auto sforx_p(F f, Cond cond)
    {
	return sforx<BEG,END,0>(MA_INC,f,cond);
    }

    template <size_t BEG, size_t END, typename F, typename Cond>
    inline auto sforx_m(F f, Cond cond)
    {
	return sforx<BEG,END,-1>(MA_DEC,f,cond);
    }

    template <size_t BEG, size_t END, typename F, typename Conc>
    inline auto sfor_p(F f, Conc conc)
    {
	return sfor<BEG,END,0>(MA_INC,f,conc);
    }

    template <size_t BEG, size_t END, typename F>
    inline auto sfor_pn(F f)
    {
	return sfor_p<BEG,END>(f,MA_ZCONC);
    }
    
    template <size_t BEG, size_t END, typename F, typename Conc>
    inline auto sfor_m(F f, Conc conc)
    {
	return sfor<BEG,END,-1>(MA_DEC,f,conc);
    }

    template <size_t BEG, size_t END, typename F>
    inline auto sfor_mn(F f)
    {
	return sfor_m<BEG,END>(f,MA_ZCONC);
    }

    template <size_t BEG, size_t END, typename F, typename Conc, typename Arg>
    inline auto sfor_p(F f, Conc conc, const Arg& arg)
    {
	return sfor<BEG,END,0>(MA_INC,f,conc,arg);
    }

    template <size_t BEG, size_t END, typename F, typename Arg>
    inline auto sfor_pn(F f, const Arg& arg)
    {
	return sfor_p<BEG,END>(f,MA_ZCONC,arg);
    }
    
    template <size_t BEG, size_t END, typename F, typename Conc, typename Arg>
    inline auto sfor_m(F f, Conc conc, const Arg& arg)
    {
	return sfor<BEG,END,-1>(MA_DEC,f,conc,arg);
    }

    template <size_t BEG, size_t END, typename F, typename Arg>
    inline auto sfor_mn(F f, const Arg& arg)
    {
	return sfor_m<BEG,END>(f,MA_ZCONC,arg);
    }

}

#define MA_SFOR(i,beg,end,incr,expr) sfor<beg,end,0>([&](auto i) constexpr { return incr; }, [&](auto i){ expr return 0; }, [&](auto f, auto next) { return 0; })
#define MA_SCFOR(i,beg,end,incr,expr,conc) sfor<beg,end,0>([&](auto i) constexpr { return incr; }, [&](auto i){ return expr; }, [&](auto f, auto next) { return f.conc(next); })
#define MA_SCFOR2(i,beg,end,incr,expr,conc) sfor<beg,end,0>([&](auto i) constexpr { return incr; }, [&](auto i){ return expr; }, [&](auto a, auto b) { return conc(a,b); })
#define MA_SCFOR3(i,beg,end,incr,expr,conc) sfor<beg,end,0>([&](auto i) constexpr { return incr; }, [&](auto i){ return expr; }, conc)
#define MA_SRFOR(i,beg,end,decr,expr) sfor<beg,end,-1>([&](auto i) constexpr { return decr; }, [&](auto i){ expr return 0; }, [&](auto f, auto next) { return 0; })
#define MA_SCRFOR(i,beg,end,decr,expr,conc) sfor<beg,end,-1>([&](auto i) constexpr { return decr; }, [&](auto i){ return expr; }, [&](auto f, auto next) { return f.conc(next); })
#define MA_SCRAFOR(i,beg,end,decr,expr,conc,arg) sfor<beg,end,-1>([&](auto i) constexpr { return decr; }, [&](auto i){ return expr; }, [&](auto f, auto next) { return f.conc(next); }, arg)
#define MA_CFOR(i,beg,end,incr,expr,cre) unpack<beg,end,0>([&](auto i) constexpr { return incr; }, [&](auto i){ expr }, [&](auto... args) { return cre(args...); })

#define MA_SCFOR_X(i,beg,end,incr,expr,conc) sfor<beg,end,0>([](auto i) constexpr { return incr; }, [](auto i){ return expr; }, [](auto f, auto next) { return f.conc(next); })

#endif
