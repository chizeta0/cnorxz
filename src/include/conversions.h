
#ifndef __ma_conversions_h__
#define __ma_conversions_h__

#include "multi_array.h"
#include "slice.h"

namespace MultiArrayTools
{

    namespace ConversionSizes
    {
	template <size_t N>
	struct OrigSize
	{
	    template <typename C, typename T>
	    struct FromTo
	    {
		static void check() { static_assert( not N % (sizeof(T) / sizeof(C)), "conversion does not fit" ); }
		static constexpr size_t SIZE = N * sizeof(T) / sizeof(C);
	    };
	};

	template <>
	struct OrigSize<-1>
	{
	    template <typename C, typename T>
	    struct FromTo
	    {
		static void check() {}
		static constexpr size_t SIZE = -1;
	    };
	};
    }

    namespace
    {
	template <typename C, typename T, size_t N>
	using SC = ConversionSizes::OrigSize<N>::template FromTo<C,T>;

	template <typename C, typename T, class Range>
	using SCR = SC<C,T,typename Range::SIZE>;

	template <typename C, typename T, class Range>
	using SCRR = GenSingleRange<typename Range::value_type,SpaceType::NONE,SCR<C,T,typename Range::SIZE>;
    }
    
    template <typename C, typename T, class Range, class... Ranges>
    Slice<C,Ranges...,SCRR<C,T,Range>> tcast(MultiArray<T,Ranges...,Range>& ma)
    {
	return Slice<C,Ranges...,SCRR<C,T,Range>>
	    ( ma.range()->space(), reinterpret_cast<C*>( ma.data() ) );
    }
    
    template <typename C, typename T, class Range, class... Ranges>
    ConstSlice<C,Ranges...,SCRR<C,T,Range>> tcast(const MultiArray<T,Ranges...,Range>& ma)
    {
	return ConstSlice<C,Ranges...,SCRR<C,T,Range>>
	    ( ma.range()->space(), reinterpret_cast<const C*>( ma.data() ) );
    }

}

#endif
