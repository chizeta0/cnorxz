
#ifndef __arith_h__
#define __arith_h__

namespace MultiArrayTools
{

    template <size_t N>
    struct ArgPack
    {
	template <class F, class Tuple, typename... As>
	static inline auto mk(const Tuple& tp, As... as)
	    -> decltype(ArgPack<N-1>::mk(tp, std::get<N>(tp), as...))
	{
	    return ArgPack<N-1>::mk(tp, std::get<N>(tp), as...);
	}
    };

    template <>
    struct ArgPack<0>
    {
	template <class F, class Tuple, typename... As>
	static inline auto mk(const Tuple& tp, As... as)
	    -> decltype(F::apply(std::get<0>(tp), as...))
	{
	    return F::apply(std::get<0>(tp), as...);
	}
    };

    template <typename T, class F, typename... As>
    struct StaticFunctionBase
    {
	static constexpr bool FISSTATIC = true;
	typedef T value_type;
	
	template <class... Ops>
	static auto mk(const Ops&... ops)
	    -> Operation<T,F,Ops...>
	{
	    return Operation<T,F,Ops...>(ops...);
	}

	static inline T apply(const std::tuple<As...>& arg)
	{
	    return ArgPack<sizeof...(As)-1>::template mk<F,std::tuple<As...> >(arg);
	}
    };
    
    // OPERATIONS (STATIC)
    template <typename T>
    struct identity : public StaticFunctionBase<T, identity<T>, T>
    {
	//static constexpr bool FISSTATIC = true;
	using StaticFunctionBase<T, identity<T>, T>::apply;
	
	static inline T apply(T a)
	{
	    return a;
	}
    };
    
    template <typename T>
    struct plus : public StaticFunctionBase<T, plus<T>, T, T>
    {
	static constexpr bool FISSTATIC = true;
	
	static inline T apply(T a1, T a2)
	{
	    return a1 + a2;
	}
    };

    template <typename T>
    struct minus : public StaticFunctionBase<T, minus<T>, T, T>
    {
	static constexpr bool FISSTATIC = true;
	
	static inline T apply(T a1, T a2)
	{
	    return a1 - a2;
	}
    };

    template <typename T>
    struct multiplies : public StaticFunctionBase<T, multiplies<T>, T, T>
    {
	static constexpr bool FISSTATIC = true;
	
	static inline T apply(T a1, T a2)
	{
	    return a1 * a2;
	}
    };

    template <typename T>
    struct divides : public StaticFunctionBase<T, divides<T>, T, T>
    {
	static constexpr bool FISSTATIC = true;
	
	static inline T apply(T a1, T a2)
	{
	    return a1 / a2;
	}
    };

#include <cmath>
#define regFunc1(fff) template <typename T>\
    struct x_##fff : public StaticFunctionBase<T, x_##fff<T>, T> {\
    static constexpr bool FISSTATIC = true;\
    static inline T apply(T a){\
	return fff(a); } };

#include "extensions/math.h"
#undef regFunc1
    
    template <size_t N>
    struct x_ipow
    {
	static constexpr bool FISSTATIC = true;

	template <typename T>
	static inline T apply(T a)
	{
	    return a * x_ipow<N-1>::apply(a);
	}
    };

    template <>
    struct x_ipow<0>
    {
	static constexpr bool FISSTATIC = true;

	template <typename T>
	static inline T apply(T a)
	{
	    return a;
	}
    };
	
    /*
    template <typename T, class Func>
    struct dynamic_function
    {
	static constexpr bool FISSTATIC = false;
	
	template <typename... Us>
	inline T apply(Us... args)
	{
	    return f(args...);
	}
    };
    */
} // end namespace MultiArrayHelper
    
#endif
