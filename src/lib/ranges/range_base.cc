
#include "ranges/range_base.h"

namespace CNORXZ
{
    
    /*************************
     *   RangeFactoryBase    *
     *************************/

    Map<SizeT,Map<Vector<PtrId>,RangePtr>> RangeFactoryBase::sCreated;
    
    RangePtr RangeFactoryBase::create()
    {
	if(not mProd) {
	    this->make();
	    mProd->mThis = mProd;
	}
	return mProd;
    }	

    RangePtr RangeFactoryBase::fromCreated(const TypeInfo& info, const Vector<PtrId>& rids) const
    {
	RangePtr out = nullptr;
	if(sCreated.count(info.hash_code()) != 0){
	    if(sCreated[info.hash_code()].count(rids) != 0){
		out = sCreated[info.hash_code()][rids];
	    }
	}
	return out;
    }

    void RangeFactoryBase::addToCreated(const TypeInfo& info, const Vector<PtrId>& rids, const RangePtr& r)
    {
	sCreated[info.hash_code()][rids] = r;
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
    
    std::intptr_t RangeBase::id() const
    {
        return reinterpret_cast<std::intptr_t>(this);
    }

} // end namespace CNORXZ
