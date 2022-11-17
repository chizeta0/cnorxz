
#include "ranges/ranges.h"

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

    RangePtr RangeBase::sub(SizeT num) const
    {
	return nullptr;
    }

    bool RangeBase::operator==(const RangeBase& in) const
    {
	return this == &in;
    }
    
    bool RangeBase::operator!=(const RangeBase& in) const
    {
	return this != &in;
    }
    
    PtrId RangeBase::id() const
    {
        return reinterpret_cast<PtrId>(this);
    }

    DIndex RangeBase::begin() const
    {
	return this->index(0);
    }
    
    DIndex RangeBase::end() const
    {
	return this->index(this->size());
    }
    
    RangePtr RangeBase::orig() const
    {
	if(mRel) return mRel;
	else return RangePtr(mThis);
    }

    RangeBase::RangeBase(const RangePtr& rel) : mRel(rel) {}

    /****************************
     *   Non-member functions   *
     ****************************/
    
    RangePtr operator*(const RangePtr& a, const RangePtr& b)
    {
	//assert(0); // check segfault + "flatten" yrange (no yrange of yranges etc)
	return YRangeFactory({a,b}).create();
    }

} // end namespace CNORXZ
