// -*- C++ -*-

#ifndef __range_factory_h__
#define __range_factory_h__

#include <cstdlib>
#include <memory>
#include "base_def.h"

namespace MultiArrayTools
{
    
    class RangeFactoryBase
    {
    public:
	DEFAULT_MEMBERS(RangeFactoryBase);
	virtual std::shared_ptr<IndefinitRangeBase> create() const = 0;
	
    };

    
} //namespace MultiArrayTools

// === NO TEMPLATE CODE HERE ===

#endif
