
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
	CXZ_ASSERT(a != nullptr, "first operand not initialized");
	CXZ_ASSERT(b != nullptr, "second operand not initialized");
	
	Vector<RangePtr> rvec;
	rvec.reserve(a->dim() + b->dim());

	if(a->sub(0) != nullptr){
	    for(SizeT i = 0; i != a->dim(); ++i){
		CXZ_ASSERT(a->sub(i) != nullptr,
			   "sub range " << i << " of first operand not available");
		rvec.push_back(a->sub(i));
	    }
	}
	else {
	    rvec.push_back(a);
	}

	if(b->sub(0) != nullptr){
	    for(SizeT i = 0; i != b->dim(); ++i){
		CXZ_ASSERT(b->sub(i) != nullptr,
			   "sub range " << i << " of second operand not available");
		rvec.push_back(b->sub(i));
	    }
	}
	else {
	    rvec.push_back(b);
	}

	return YRangeFactory(rvec).create();
    }

} // end namespace CNORXZ
