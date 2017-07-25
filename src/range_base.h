// -*- C++ -*-

#ifndef __range_base_h__
#define __range_base_h__

#include <cstdlib>
#include <vector>
#include <memory>

#include "base_def.h"

namespace MultiArrayTools
{
    
    enum class RangeType
    {
	NIL = 0,
	ANY = 1,
	SPACE = 2,
	MOMENTUM = 3,
	LORENTZ = 4,
	SPIN = 5,
	ENSEMBLE = 6,
	VALUE_ERROR = 7,
	DISTANCE = 8
    };

    class RangeBase
    {
    public:

	virtual ~RangeBase() = default;

	virtual size_t size() const = 0;
	virtual size_t dim() const = 0;
	
	virtual std::shared_ptr<IndexBase> index() const = 0;
	
    protected:
	
    };

    template <class Index>
    class RangeInterface : public RangeBase
    {
    public:

	typedef Index IndexType;

	virtual Index begin() = 0;
	virtual Index end() = 0;
    };
    
}

#include "range_base.cc"

#endif
