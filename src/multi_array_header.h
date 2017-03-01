// -*- C++ -*-

#ifndef __multi_array_header_h__
#define __multi_array_header_h__

#include <cstdlib>
#include "base_def.h"
#include "range_base.h"
#include "index_base.h"
#include "single_range.h"
#include "multi_range.h"
#include "multi_array_operation.h"
#include "multi_array.h"
#include "slice.h"

namespace MultiArrayTools
{
    
    /*********************************
     *   Some standard definitions   *
     *********************************/

    // ===== Index Types =====

    typedef SingleIndex<size_t,RangeType::ANY> GenericNI;
    typedef SingleIndex<double,RangeType::ANY> GenericFI;
    typedef SingleIndex<size_t,RangeType::LORENTZ> LorentzI;
    typedef SingleIndex<int,RangeType::SPACE> Space1dNI;
    typedef SingleIndex<int,RangeType::MOMENTUM> Mom1dNI;
    // ...

    // ===== Range Types =====

    typedef SingleRange<size_t,RangeType::ANY> GenericNR;
    typedef SingleRange<double,RangeType::ANY> GenericFR;
    typedef SingleRange<size_t,RangeType::LORENTZ> LorentzR;
    typedef SingleRange<int,RangeType::SPACE> Space1dNR;
    typedef SingleRange<int,RangeType::MOMENTUM> Mom1dNR;
    typedef MultiRange<Space1dNR,Space1dNR,Space1dNR> Space3dNR;
    typedef MultiRange<Mom1dNR,Mom1dNR,Mom1dNR> Mom3dNR;
    // ...
    
}

#endif
