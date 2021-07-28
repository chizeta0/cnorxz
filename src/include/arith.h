
#ifndef __arith_h__
#define __arith_h__

#include <functional>

namespace CNORXZ
{

    //template <typename T, class F, typename... As>
    template <class F>
    struct StaticFunctionBase
    {
	static constexpr bool FISSTATIC = true;
	typedef F function;
	//typedef typename F::value_type value_type;

	template <class... Ops>
	static auto mk(const Ops&... ops);

        template <size_t N, class Tuple, typename... As>
        static inline auto xapply(const Tuple& tp, As... as);

        template <typename... As>
	static inline auto apply(const std::tuple<As...>& arg);
    };


    // OPERATIONS (STATIC)
    template <typename T>
    struct identity : public StaticFunctionBase<identity<T>>
    {
	//static constexpr bool FISSTATIC = true;
	using StaticFunctionBase<identity<T>>::apply;
        typedef T value_type;
        
	static inline T apply(T a)
	{
	    return a;
	}

	static inline T selfApply(T& a1, const T& a2)
	{
	    return a1 = a2;
	}
    };

    template <typename T, typename U>
    using plusv = decltype(std::declval<T>()+std::declval<U>());

    template <typename T, typename U>
    using minusv = decltype(std::declval<T>()-std::declval<U>());

    template <typename T, typename U>
    using multipliesv = decltype(std::declval<T>()*std::declval<U>());

    template <typename T, typename U>
    using dividesv = decltype(std::declval<T>()/std::declval<U>());

    template <typename T, typename U>
    struct plusx : public StaticFunctionBase<plusx<T,U>>
    {
	static constexpr bool FISSTATIC = true;
	using StaticFunctionBase<plusx<T,U>>::apply;
        typedef plusv<T,U> value_type;
        
	static inline value_type apply(T a1, U a2)
	{
	    return a1 + a2;
	}

    	static inline T& selfApply(T& a1, const T& a2)
	{
	    return a1 += a2;
	}
    };

    template <typename T, typename U>
    struct minusx : public StaticFunctionBase<minusx<T,U>>
    {
	static constexpr bool FISSTATIC = true;
	using StaticFunctionBase<minusx<T,U>>::apply;
        typedef minusv<T,U> value_type;
	
	static inline value_type apply(T a1, U a2)
	{
	    return a1 - a2;
	}
    };

    template <typename T, typename U>
    struct multipliesx : public StaticFunctionBase<multipliesx<T,U>>
    {
	static constexpr bool FISSTATIC = true;
	using StaticFunctionBase<multipliesx<T,U>>::apply;
        typedef multipliesv<T,U> value_type;
	
	static inline value_type apply(T a1, U a2)
	{
	    return a1 * a2;
	}
    };

    template <typename T, typename U>
    struct dividesx : public StaticFunctionBase<dividesx<T,U>>
    {
	static constexpr bool FISSTATIC = true;
	using StaticFunctionBase<dividesx<T,U>>::apply;
        typedef dividesv<T,U> value_type;
	
	static inline value_type apply(T a1, U a2)
	{
	    return a1 / a2;
	}

    };

    template <typename T>
    struct negate : public StaticFunctionBase<negate<T>>
    {
	static constexpr bool FISSTATIC = true;
	using StaticFunctionBase<negate<T>>::apply;
        typedef T value_type;
        
	static inline T apply(T a)
	{
	    return -a;
	}

    };

    template <typename T>
    using plus = plusx<T,T>;

    template <typename T>
    using minus = minusx<T,T>;
    
    template <typename T>
    using multiplies = multipliesx<T,T>;

    template <typename T>
    using divides = dividesx<T,T>;

    //  OPERATIONS (STATIC)
    template <typename R, typename... Args>
    class function
    {
    public:
        static constexpr bool FISSTATIC = false;

    private:
        std::function<R(Args...)> mF;

    public:

        function() = default;
        function(const std::function<R(Args...)>& in) : mF(in) {}
        
        inline R operator()(const Args&... args)
        {
            return mF(args...);
        }

        template <size_t N, class Tuple, typename... As>
        static inline auto xapply(const std::function<R(Args...)>& ff, const Tuple& tp, As... as)
        {
            if constexpr(N > 0){
                return xapply<N-1>(ff, tp, std::get<N>(tp), as...);
            }
            else {
                return ff(std::get<0>(tp), as...);
            }
        }

        inline R operator()(const std::tuple<Args...>& args)
        {
            return xapply<sizeof...(Args)-1>(mF, args);
        }
};
    
#include <cmath>
#define regFunc1(fff) template <typename T>\
    struct x_##fff : public StaticFunctionBase<x_##fff<T>> {\
    static constexpr bool FISSTATIC = true;\
    typedef T value_type; \
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
            if constexpr(N > 0){
                return a * x_ipow<N-1>::apply(a);
            }
            else {
                return a;
            }
	}
    };

} // end namespace CNORXZInternal

#include "arith.cc.h"

#endif
