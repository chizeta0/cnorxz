
#ifndef __arith_h__
#define __arith_h__

namespace MultiArrayHelper
{
    // OPERATIONS (STATIC)
    template <typename T>
    struct plus
    {
	static constexpr bool FISSTATIC = true;
	
	static inline T apply(T a1, T a2)
	{
	    return a1 + a2;
	}
    };

    template <typename T>
    struct minus
    {
	static constexpr bool FISSTATIC = true;
	
	static inline T apply(T a1, T a2)
	{
	    return a1 - a2;
	}
    };

    template <typename T>
    struct multiplies
    {
	static constexpr bool FISSTATIC = true;
	
	static inline T apply(T a1, T a2)
	{
	    return a1 * a2;
	}
    };

    template <typename T>
    struct divides
    {
	static constexpr bool FISSTATIC = true;
	
	static inline T apply(T a1, T a2)
	{
	    return a1 / a2;
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
