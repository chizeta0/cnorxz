
#include "ranges/range_base.h"

namespace MultiArrayTools
{

    size_t indexId()
    {
	static size_t id = 0;
	++id;
	return id;
    }
    
    /*************************
     *   RangeFactoryBase    *
     *************************/
    
    void RangeFactoryBase::setSelf()
    {
	mProd->mThis = mProd;
    }
    
    /******************
     *   RangeBase    *
     ******************/

    bool RangeBase::operator==(const RangeBase& in) const
    {
	return this == &in;
    }
    
    bool RangeBase::operator!=(const RangeBase& in) const
    {
	return this != &in;
    }


} // end namespace MultiArrayTools
