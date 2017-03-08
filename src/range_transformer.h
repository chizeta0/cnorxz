// -*- C++ -*-

#ifndef __range_transformer_h__
#define __range_transformer_h__

#include <cstdlib>
#include "base_def.h"

namespace MultiArrayTools
{

    template <class... Ranges>
    MultiRange<Ranges...> makeRange(const std::tuple<Ranges...>& rangeTuple);
    
    template <class... Ranges>
    MultiRange<Ranges...> combine(const Ranges&... ranges);

    template <class Range, class... Ranges>
    auto extend(const Range& r, const MultiRange<Ranges...>& mr)
	-> decltype( MultiRange<Range,Ranges...>(std::tuple_cat( std::make_tuple(r), mr.space() ) ) );
    
    template <class MultiRange1, class MultiRange2>
    auto merge(const MultiRange1& mr1, const MultiRange2& mr2)
	-> decltype(makeRange( std::tuple_cat( mr1.space(), mr2.space() ) ) );
}

#include "range_transformer.cc"

#endif
