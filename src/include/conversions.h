
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
	struct OrigSize<MUI>
	{
	    template <typename C, typename T>
	    struct FromTo
	    {
		static void check() {}
		static constexpr size_t SIZE = MUI;
	    };
	};
    }

    namespace
    {
	template <typename C, typename T, size_t N>
	using SC = typename ConversionSizes::OrigSize<N>::template FromTo<C,T>;

	template <typename C, typename T, class Range>
	using SCR = SC<C,T,Range::SIZE>;

	template <typename C, typename T, class Range>
	using SCRR = GenSingleRange<typename Range::MetaType,SpaceType::NONE,SCR<C,T,Range>::SIZE>;
    }

    template <size_t N>
    struct SubTuple
    {
	template <class RTP, class... Ranges>
	static inline auto mk(const RTP& rtp, const Ranges&... rs)
	    -> decltype(SubTuple<N-1>::mk(rtp, std::get<N>(rtp), rs...))
	{
	    return SubTuple<N-1>::mk(rtp, std::get<N>(rtp), rs...);
	}
    };

    template <>
    struct SubTuple<0>
    {
	template <class RTP, class... Ranges>
	static inline auto mk(const RTP& rtp, const Ranges&... rs)
	    -> decltype(std::make_tuple(std::get<0>(rtp), rs...))
	{
	    return std::make_tuple(std::get<0>(rtp), rs...);
	}
    };

    template <class... Ranges>
    using LastR = typename std::tuple_element<sizeof...(Ranges)-1,std::tuple<Ranges...>>::type;
    
    template <typename C, typename T, class... Ranges>
    auto rtcast(const std::tuple<std::shared_ptr<Ranges>...>& rtp)
	-> decltype(std::tuple_cat(SubTuple<sizeof...(Ranges)-2>::mk(rtp),
				   std::make_tuple( std::dynamic_pointer_cast<SCRR<C,T,LastR<Ranges...>>>
						    ( SCRR<C,T,LastR<Ranges...>>::factory().create() ) ) ))
    {
	return std::tuple_cat(SubTuple<sizeof...(Ranges)-2>::mk(rtp),
				   std::make_tuple( std::dynamic_pointer_cast<SCRR<C,T,LastR<Ranges...>>>
						    ( SCRR<C,T,LastR<Ranges...>>::factory().create() ) ) );
    }

    template <typename T, class... Ranges>
    inline Slice<T,Ranges...> rangeTpToSlice( const std::tuple<std::shared_ptr<Ranges>...>& rtp, T* data )
    {
	return Slice<T,Ranges...>(rtp, data);
    }

    template <typename T, class... Ranges>
    inline ConstSlice<T,Ranges...> rangeTpToSlice( const std::tuple<std::shared_ptr<Ranges>...>& rtp, const T* data )
    {
	return ConstSlice<T,Ranges...>(rtp, data);
    }

    template <typename C, typename T, class... Ranges>
    auto tcast(MultiArray<T,Ranges...>& ma)
	-> decltype(rangeTpToSlice
	    ( rtcast<C,T>( ma.range()->space() ),
	      reinterpret_cast<C*>( ma.data() ) ))
    {   
	// VCHECK(reinterpret_cast<std::intptr_t>(ma.data()) % 32);
	//VCHECK(reinterpret_cast<std::intptr_t>(reinterpret_cast<C*>(ma.data())) % 32);
	return rangeTpToSlice
	    ( rtcast<C,T>( ma.range()->space() ),
	      reinterpret_cast<C*>( ma.data() ) );
    }
    
    template <typename C, typename T, class... Ranges>
    auto tcast(const MultiArray<T,Ranges...>& ma)
	-> decltype(rangeTpToSlice
	    ( rtcast<C,T>( ma.range()->space() ),
	      reinterpret_cast<const C*>( ma.data() ) ))
    {
	//VCHECK(reinterpret_cast<std::intptr_t>(ma.data()) % 32);
	//VCHECK(reinterpret_cast<std::intptr_t>(reinterpret_cast<C*>(ma.data())) % 32);
	return rangeTpToSlice
	    ( rtcast<C,T>( ma.range()->space() ),
	      reinterpret_cast<const C*>( ma.data() ) );
    }
}
#endif
