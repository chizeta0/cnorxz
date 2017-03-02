// -*- C++ -*-

#ifndef __range_transformer_h__
#define __range_transformer_h__

#include <cstdlib>
#include "base_def.h"

namespace MultiArrayTools
{

    // init 'MultiRange' with 'new'
    template <class... Ranges>
    shared_ptr<MultiRange<Ranges...> > combine(const Ranges&... ranges);

}

#include "range_transformer.cc"

#endif
