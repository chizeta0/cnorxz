
#ifndef __arith_h__
#define __arith_h__

namespace MultiArrayHelper
{
    // OPERATIONS (STATIC)
    template <typename T>
    struct plus
    {
	static inline T apply(T a1, T a2)
	{
	    return a1 + a2;
	}
    };

    template <typename T>
    struct minus
    {
	static inline T apply(T a1, T a2)
	{
	    return a1 - a2;
	}
    };

    template <typename T>
    struct multiplies
    {
	static inline T apply(T a1, T a2)
	{
	    return a1 * a2;
	}
    };

    template <typename T>
    struct divides
    {
	static inline T apply(T a1, T a2)
	{
	    return a1 / a2;
	}
    };

    template <typename T, class Func>
    struct dynamic_function
    {
	template <typename... Us>
	static inline T apply(const Func& f, Us... args)
	{
	    return f(args...);
	}
    }
    
} // end namespace MultiArrayHelper
    
#endif
