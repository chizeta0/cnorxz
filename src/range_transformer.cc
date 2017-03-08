// -*- C++ -*-

#include "range_transformer.h"

namespace MultiArrayTools
{

    template <class... Ranges>
    MultiRange<Ranges...> makeRange(const std::tuple<Ranges...>& rangeTuple)
    {
	return MultiRange<Ranges...>( rangeTuple );
    }

    template <class... Ranges>
    MultiRange<Ranges...> combine(const Ranges&... ranges)
    {
	return MultiRange<Ranges...>(ranges...);
    }

    template <class Range, class... Ranges>
    auto extend(const Range& r, const MultiRange<Ranges...>& mr)
	-> decltype( MultiRange<Range,Ranges...>(std::tuple_cat( std::make_tuple(r), mr.space() ) ) )
    {
	return MultiRange<Range,Ranges...>(std::tuple_cat( std::make_tuple(r), mr.space() ) );
    }
    
    template <class MultiRange1, class MultiRange2>
    auto merge(const MultiRange1& mr1, const MultiRange2& mr2)
	-> decltype( makeRange( std::tuple_cat( mr1.space(), mr2.space() ) ) )
    {
	return makeRange( std::tuple_cat( mr1.space(), mr2.space() ) );
    }
    
}
