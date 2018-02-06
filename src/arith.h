
#ifndef __arith_h__
#define __arith_h__

namespace MultiArrayHelper
{
    // OPERATIONS (STATIC)
    template <typename T>
    struct plus
    {
	static inline T& acc(T& target, const T& arg)
	{
	    return target += arg;
	}
    };

    template <typename T>
    struct minus
    {
	static inline T& acc(T& target, const T& arg)
	{
	    return target -= arg;
	}
    };

    template <typename T>
    struct multiplies
    {
	static inline T& acc(T& target, const T& arg)
	{
	    return target *= arg;
	}
    };

    template <typename T>
    struct divides
    {
	static inline T& acc(T& target, const T& arg)
	{
	    return target /= arg;
	}
    };


} // end namespace MultiArrayHelper
    
#endif
