// -*- C++ -*-

#ifndef __range_base_h__
#define __range_base_h__

#include <cstdlib>
#include <vector>
#include <memory>

#include "base_def.h"

namespace MultiArrayTools
{
    class RangeBase;
}
#include "vindex_base.h"

namespace MultiArrayTools
{

    size_t indexId();
    
    enum class SpaceType
    {
	NONE = 0,
	ANY = 1,
#define include_range_type(x,n) x = n,
#include "range_types/header.h"
#undef include_range_type
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
	
	virtual std::shared_ptr<VIWB> index() const = 0;

	bool operator==(const RangeBase& in) const;
	bool operator!=(const RangeBase& in) const;

	//virtual bool regular() const = 0; // integer distance (e.g. 2,3,4,...)
	//virtual bool linear() const = 0; // 1dim valuable (e.g. 2.45, 3.12, 3.56,...)
	//virtual bool multi() const = 0; // mdim
	//virtual bool maplike() const = 0; // meta type is ~ MultiArray<T,...>
	
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

#endif
