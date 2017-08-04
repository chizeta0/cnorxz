
#include "range_base.h"

namespace MultiArrayTools
{

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

}
