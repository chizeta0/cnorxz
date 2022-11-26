
#ifndef __cxz_reg_cc_h__
#define __cxz_reg_cc_h__

#include "reg.h"
#include "xpr/pos_type.h"

namespace CNORXZ
{
    template <typename T, class EPosT, SizeT... Is>
    inline decltype(auto) vregi(const T* d, const EPosT& pos, std::index_sequence<Is...> is)
    {
	constexpr SizeT N = epos_size<EPosT>::value;
	static_assert(N == sizeof...(Is), "got inconsistent index sequence");
	return Consecutive<T,N> { d[pos.template get<Is>().val()]... };
    }

    template <typename T, class EPosT>
    inline decltype(auto) vreg(const T* d, const EPosT& pos)
    {
	constexpr SizeT N = epos_size<EPosT>::value;
	static_assert(is_epos_type<EPosT>::value, "got non-epos-type");
	if constexpr(pos_type_is_consecutive<EPosT>::value){
	    return *reinterpret_cast<const Consecutive<T,N>*>(d);
	}
	else {
	    return vregi(d, pos, std::make_index_sequence<N>{});
	}
    }

    template <typename T, class EPosT>
    inline decltype(auto) vreg(T* d, const EPosT& pos)
    {
	constexpr SizeT N = epos_size<EPosT>::value;
	static_assert(is_epos_type<EPosT>::value, "got non-epos-type");
	if constexpr(pos_type_is_consecutive<EPosT>::value){
	    return *reinterpret_cast<Consecutive<T,N>*>(d);
	}
	else {
	    return vregi(d, pos, std::make_index_sequence<N>{});
	}
    }

    template <SizeT I, typename T>
    constexpr decltype(auto) consecGet(const T& a)
    {
	if constexpr(is_consecutive_type<T>::value){
	    static_assert(I < consecutive_size<T>::value,
			  "consecutive index out of range");
	    return a.mD[I];
	}
	else {
	    return a;
	}
    }

    template <SizeT I, typename T>
    constexpr decltype(auto) consecGet(T& a)
    {
	if constexpr(is_consecutive_type<T>::value){
	    static_assert(I < consecutive_size<T>::value,
			  "consecutive index out of range");
	    return a.mD[I];
	}
	else {
	    return a;
	}
    }

    template <SizeT I, class F, typename... Args>
    constexpr decltype(auto) consecApply(const F& f, const Args&... args)
    {
	return f( consecGet<I>(args)... );
    }

    template <SizeT I, class F, typename Dst, typename... Args>
    constexpr Dst& consecAssign(const F& f, Dst& dst, const Args&... args)
    {
	f( consecGet<I>(dst), consecGet<I>(args)... );
	return dst;
    }

    template <class F, typename... Args, SizeT... Is>
    static constexpr decltype(auto) consecFuncI(const F& f, const Args&... args,
						std::index_sequence<Is...> is)
    {
	typedef decltype(consecApply<0>(f, args...)) OType;
	constexpr SizeT N = sizeof...(Is);
	return Consecutive<OType,N> { consecApply<Is>(f, args...) ... };
    }

    template <class F, typename Dst, typename... Args, SizeT... Is>
    constexpr Dst& consecFuncAI(const F& f, Dst& dst, const Args&... args,
				std::index_sequence<Is...> is)
    {
	( consecAssign<Is>(f, dst, args...), ... );
	return dst;
    }

    template <SizeT N, class F, typename... Args>
    constexpr decltype(auto) consecFunc(const F& f, const Args&... args)
    {
	return consecFuncI<F,Args...>(f, args..., std::make_index_sequence<N>{});
    }

    template <SizeT N, class F, typename Dst, typename... Args>
    constexpr Dst& consecFuncA(const F& f, Dst& dst, const Args&... args)
    {
	return consecFuncAI<F,Dst,Args...>(f, dst, args..., std::make_index_sequence<N>{});
    }

    /******************************
     *   basic operations: plus   *
     ******************************/

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto)
    PlusCC<T,U,N>::eval(const Consecutive<T,N>& a, const Consecutive<U,N>& b)
    {
	return consecFunc( [](const auto& x, const auto& y) { return x + y; }, a, b );
    }

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto)
    PlusCC<T,U,N>::aeval(Consecutive<T,N>& o, const Consecutive<U,N>& a)
    {
	return consecFuncA( [](auto& x, const auto& y) { return x += y; }, o, a );
    }

    template <typename T, typename X, SizeT N>
    constexpr decltype(auto) PlusCX<T,X,N>::eval(const Consecutive<T,N>& a, const X& b)
    {
	return consecFunc( [](const auto& x, const auto& y) { return x + y; }, a, b );
    }

    template <typename T, typename X, SizeT N>
    constexpr decltype(auto) PlusCX<T,X,N>::eval(const X& a, const Consecutive<T,N>& b)
    {
	return consecFunc( [](const auto& x, const auto& y) { return x + y; }, a, b );
    }

    template <typename T, typename X, SizeT N>
    constexpr decltype(auto) PlusCX<T,X,N>::aeval(Consecutive<T,N>& o, const X& a)
    {
	return consecFuncA( [](auto& x, const auto& y) { return x += y; }, o, a );
    }

    /*******************************
     *   basic operations: minus   *
     *******************************/

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) MinusCC<T,U,N>::eval(const Consecutive<T,N>& a, const Consecutive<U,N>& b)
    {
	return consecFunc( [](const auto& x, const auto& y) { return x - y; }, a, b );
    }

    template <typename T, typename X, SizeT N>
    constexpr decltype(auto) MinusCX<T,X,N>::eval(const Consecutive<T,N>& a, const X& b)
    {
	return consecFunc( [](const auto& x, const auto& y) { return x - y; }, a, b );
    }

    template <typename T, typename X, SizeT N>
    constexpr decltype(auto) MinusCX<T,X,N>::eval(const X& a, const Consecutive<T,N>& b)
    {
	return consecFunc( [](const auto& x, const auto& y) { return x - y; }, a, b );
    }

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) MinusCC<T,U,N>::aeval(Consecutive<T,N>& o, const Consecutive<U,N>& a)
    {
	return consecFuncA( [](auto& x, const auto& y) { return x -= y; }, o, a );
    }

    template <typename T, typename X, SizeT N>
    constexpr decltype(auto) MinusCX<T,X,N>::aeval(Consecutive<T,N>& o, const X& a)
    {
	return consecFuncA( [](auto& x, const auto& y) { return x -= y; }, o, a );
    }

    /***********************************
     *   basic operations: muliplies   *
     ***********************************/

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) MultipliesCC<T,U,N>::eval(const Consecutive<T,N>& a, const Consecutive<U,N>& b)
    {
	return consecFunc( [](const auto& x, const auto& y) { return x * y; }, a, b );
    }

    template <typename T, typename X, SizeT N>
    constexpr decltype(auto) MultipliesCX<T,X,N>::eval(const Consecutive<T,N>& a, const X& b)
    {
	return consecFunc( [](const auto& x, const auto& y) { return x * y; }, a, b );
    }

    template <typename T, typename X, SizeT N>
    constexpr decltype(auto) MultipliesCX<T,X,N>::eval(const X& a, const Consecutive<T,N>& b)
    {
	return consecFunc( [](const auto& x, const auto& y) { return x * y; }, a, b );
    }

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) MultipliesCC<T,U,N>::aeval(Consecutive<T,N>& o, const Consecutive<U,N>& a)
    {
	return consecFuncA( [](const auto& x, const auto& y) { return x *= y; }, o, a );
    }

    template <typename T, typename X, SizeT N>
    constexpr decltype(auto) MultipliesCX<T,X,N>::aeval(Consecutive<T,N>& o, const X& a)
    {
	return consecFuncA( [](const auto& x, const auto& y) { return x *= y; }, o, a );
    }

    /*********************************
     *   basic operations: divides   *
     *********************************/

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) DividesCC<T,U,N>::eval(const Consecutive<T,N>& a, const Consecutive<U,N>& b)
    {
	return consecFunc( [](const auto& x, const auto& y) { return x / y; }, a, b );
    }

    template <typename T, typename X, SizeT N>
    constexpr decltype(auto) DividesCX<T,X,N>::eval(const Consecutive<T,N>& a, const X& b)
    {
	return consecFunc( [](const auto& x, const auto& y) { return x / y; }, a, b );
    }

    template <typename T, typename X, SizeT N>
    constexpr decltype(auto) DividesCX<T,X,N>::eval(const X& a, const Consecutive<T,N>& b)
    {
	return consecFunc( [](const auto& x, const auto& y) { return x / y; }, a, b );
    }

    template <typename T, typename U, SizeT N>
    constexpr decltype(auto) DividesCC<T,U,N>::aeval(Consecutive<T,N>& o, const Consecutive<U,N>& a)
    {
	return consecFuncA( [](const auto& x, const auto& y) { return x /= y; }, o, a );
    }

    template <typename T, typename X, SizeT N>
    constexpr decltype(auto) DividesCX<T,X,N>::aeval(Consecutive<T,N>& o, const X& a)
    {
	return consecFuncA( [](const auto& x, const auto& y) { return x /= y; }, o, a );
    }
}

#endif
