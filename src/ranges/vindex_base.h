
#ifndef __vindex_base_h__
#define __vindex_base_h__

//#include "ranges/range_base.h"
#include "ranges/index_type.h"

namespace MultiArrayTools
{

#ifndef __range_base_h__
    class RangeBase;
#endif
    
    class VirtualIndexWrapperBase
    {
    public:
	DEFAULT_MEMBERS(VirtualIndexWrapperBase);
	
	virtual IndexType type() const = 0;
	virtual size_t dim() const = 0;
	virtual size_t pos() const = 0;
	virtual size_t max() const = 0;
	virtual std::shared_ptr<RangeBase> rangePtr() const = 0;
	virtual std::shared_ptr<VirtualIndexWrapperBase> getPtr(size_t n) const = 0;
	virtual std::intptr_t getPtrNum() const = 0;
	virtual size_t getStepSize(size_t n) const = 0;
    };

    typedef VirtualIndexWrapperBase VIWB;

} // end namespace MultiArrayTools

#endif
