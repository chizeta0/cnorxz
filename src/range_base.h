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

    class RangeFactoryBase
    {
    public:

	RangeFactoryBase() = default;
	virtual ~RangeFactoryBase() = default;

	// should return mProd !!
	virtual std::shared_ptr<RangeBase> create() = 0;
	
    protected:

	std::shared_ptr<RangeBase> mProd;
	
	// call this function before returning product !!
	void setSelf();
    };
    
    class RangeBase
    {
    public:

	virtual ~RangeBase() = default;
	    
	virtual size_t size() const = 0;
	virtual size_t dim() const = 0;
	
	virtual std::shared_ptr<IndexBase> index() const = 0;

	bool operator==(const RangeBase& in) const;
	bool operator!=(const RangeBase& in) const;
	
	friend RangeFactoryBase;
	
    protected:

	RangeBase() = default;
	std::weak_ptr<RangeBase> mThis;
    };

    template <class Index>
    class RangeInterface : public RangeBase
    {
    public:

	typedef Index IndexType;

	virtual Index begin() const = 0;
	virtual Index end() const = 0;

    protected:
	RangeInterface() = default;
    };
    
}

#include "range_base.cc"

#endif
