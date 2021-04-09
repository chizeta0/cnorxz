
#ifndef __static_for_h__
#define __static_for_h__

#include <cstdlib>

namespace MultiArrayTools
{

    template <size_t BEG, size_t END, size_t OFF>
    struct sfor
    {
	template <typename Incr, typename F, typename Conc>
	static inline auto exec(Incr incr, F f, Conc conc)
	{
	    constexpr auto idx = std::integral_constant<size_t, BEG>{};
	    constexpr auto idxm = std::integral_constant<size_t, BEG+OFF>{};
	    static_assert(abs(idx.value - END) >= abs(incr(idx) - END),
			  "this turns out to be a static endless loop");
	    auto tmp = f(idxm);
	    return conc(tmp, static_for<incr(idx),END,OFF>::exec(incr,f,conc));
	}
	
	template <typename Incr, typename F, typename Create, typename... Args>
	static inline auto unpack(Incr incr, F f, Create create, const Args&... args)
	{
	    constexpr auto idx = std::integral_constant<size_t, BEG>{};
	    constexpr auto idxm = std::integral_constant<size_t, BEG+OFF>{};
	    static_assert(abs(idx.value - END) >= abs(incr(idx) - END),
			  "this turns out to be a static endless loop");
	    auto tmp = f(idxm);
	    return static_for<incr(idx),END,OFF>::unpack(incr, f, create, args..., tmp);
	}
    };

    template <size_t END, size_t OFF>
    struct sfor<END,END,OFF>
    {
	template <typename Incr, typename F, typename Conc>
	static inline auto exec(Incr incr, F f, Conc conc)
	{
	    return 0;
	}

	template <typename Incr, typename F, typename Create, typename... Args>
	static inline auto unpack(Incr incr, F f, Create create, const Args&... args)
	{
	    return create(args...);
	}
    };
}

#define MA_SFOR(i,beg,end,incr,expr) static_for<beg,end,0>::exec([&](auto i) constexpr { return incr; }, [&](auto i){ expr return 0; }, [&](auto f, auto next) { return 0; })
#define MA_SRFOR(i,beg,end,decr,expr) static_for<beg,end,-1>::exec([&](auto i) constexpr { return decr; }, [&](auto i){ expr return 0; }, [&](auto f, auto next) { return 0; })
#define MA_CFOR(i,beg,end,incr,expr,cre) static_for<beg,end,0>::unpack([&](auto i) constexpr { return incr; }, [&](auto i){ expr }, [&](auto... args) { return cre(args...); })

#endif
