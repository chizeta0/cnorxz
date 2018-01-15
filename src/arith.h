
#ifndef __arith_h__
#define __arith_h__

namespace MultiArrayHelper
{
    // OPERATIONS (STATIC)
    template <typename T>
    struct plus
    {
	static inline T&& apply(const T& a1, const T& a2)
	{
	    T&& res = a1 + a2;
	    return std::forward<T>( res );
	}
	
	static inline T&& apply(T&& a1, const T& a2)
	{
	    T&& res = a1 + a2;
	    return std::forward<T>( res );
	}

	static inline T&& apply(const T& a1, T&& a2)
	{
	    T&& res = a1 + a2;
	    return std::forward<T>( res );
	}

	static inline T&& apply(T&& a1, T&& a2)
	{
	    T&& res = a1 + a2;
	    return std::forward<T>( res );
	}

    };

    template <typename T>
    struct minus
    {
	static inline T&& apply(const T& a1, const T& a2)
	{
	    T&& res = a1 - a2;
	    return std::forward<T>( res );
	}
	
	static inline T&& apply(T&& a1, const T& a2)
	{
	    T&& res = a1 - a2;
	    return std::forward<T>( res );
	}

	static inline T&& apply(const T& a1, T&& a2)
	{
	    T&& res = a1 - a2;
	    return std::forward<T>( res );
	}

	static inline T&& apply(T&& a1, T&& a2)
	{
	    T&& res = a1 - a2;
	    return std::forward<T>( res );
	}
    };

    template <typename T>
    struct multiplies
    {
	static inline T&& apply(const T& a1, const T& a2)
	{
	    T&& res = a1 * a2;
	    return std::forward<T>( res );
	}
	
	static inline T&& apply(T&& a1, const T& a2)
	{
	    T&& res = a1 * a2;
	    return std::forward<T>( res );
	}

	static inline T&& apply(const T& a1, T&& a2)
	{
	    T&& res = a1 * a2;
	    return std::forward<T>( res );
	}

	static inline T&& apply(T&& a1, T&& a2)
	{
	    T&& res = a1 * a2;
	    return std::forward<T>( res );
	}
    };

    template <typename T>
    struct divides
    {
	static inline T&& apply(const T& a1, const T& a2)
	{
	    T&& res = a1 / a2;
	    return std::forward<T>( res );
	}
	
	static inline T&& apply(T&& a1, const T& a2)
	{
	    T&& res = a1 / a2;
	    return std::forward<T>( res );
	}

	static inline T&& apply(const T& a1, T&& a2)
	{
	    T&& res = a1 / a2;
	    return std::forward<T>( res );
	}

	static inline T&& apply(T&& a1, T&& a2)
	{
	    T&& res = a1 / a2;
	    return std::forward<T>( res );
	}
    };


} // end namespace MultiArrayHelper
    
#endif
