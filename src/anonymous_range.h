// -*- C++ -*-

#ifndef __anonymous_range_h__
#define __anonymous_range_h__

#include <cstdlib>
#include "base_def.h"

namespace MultiArrayTools
{

    typedef SingleIndex<size_t,NONE> AnonymousIndex;
    
    class AnonymousRange : public RangeBase<AnonymousIndex>
    {

    };

}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

// ...

#endif
