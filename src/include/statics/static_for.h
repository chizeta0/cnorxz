
#ifndef __static_for_h__
#define __static_for_h__

#include <cstdlib>

namespace MultiArrayTools
{

    template <size_t BEG, size_t END>
    struct sfor
    {
	template <typename F>
	static void execs(const F& f)
	{
	    if(BEG < END){
		f(std::integral_constant<size_t, BEG>{});
		sfor<BEG+1,END>::execs(f);
	    }
	}

	template <typename F>
	static void execsr(const F& f)
	{
	    if(BEG < END){
		f(std::integral_constant<size_t, END-1>{});
		sfor<BEG,END-1>::execsr(f);
	    }
	}

	template <typename F>
	static void exec(const F& f)
	{
	    f(std::integral_constant<size_t, BEG>{});
	    sfor<BEG+1,END>::exec(f);
	}

	template <typename F>
	static void execr(const F& f)
	{
	    f(std::integral_constant<size_t, END-1>{});
	    sfor<BEG,END-1>::execr(f);
	}
    };

    template <size_t N>
    struct sfor<N,N>
    {
	template <typename F>
	static void execs(const F& f) {}

    	template <typename F>
	static void execsr(const F& f) {}

    	template <typename F>
	static void exec(const F& f) {}

    	template <typename F>
	static void execr(const F& f) {}
    };
}

#define MA_SSFOR(i,beg,end,expr) MultiArrayTools::sfor<beg,end>::execs([&](auto i){ expr })
#define MA_SSRFOR(i,end,beg,expr) MultiArrayTools::sfor<beg,end>::execsr([&](auto i){ expr })
#define MA_SFOR(i,beg,end,expr) MultiArrayTools::sfor<beg,end>::exec([&](auto i){ expr })
#define MA_SRFOR(i,end,beg,expr) MultiArrayTools::sfor<beg,end>::execr([&](auto i){ expr })

#endif
