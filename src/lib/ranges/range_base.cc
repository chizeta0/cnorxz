
#include "ranges/ranges.h"
#include "array/array.h"

namespace CNORXZ
{
    
    /*************************
     *   RangeFactoryBase    *
     *************************/

    Map<SizeT,Map<Vector<Uuid>,RangePtr>> RangeFactoryBase::sCreated;
    
    RangePtr RangeFactoryBase::create()
    {
	if(not mProd) {
	    this->make();
	    mProd->mThis = mProd;
	}
	return mProd;
    }	

    RangePtr RangeFactoryBase::fromCreated(const TypeInfo& info, const Vector<Uuid>& rids) const
    {
	RangePtr out = nullptr;
	if(sCreated.count(info.hash_code()) != 0){
	    if(sCreated[info.hash_code()].count(rids) != 0){
		out = sCreated[info.hash_code()][rids];
	    }
	}
	return out;
    }

    void RangeFactoryBase::addToCreated(const TypeInfo& info, const Vector<Uuid>& rids, const RangePtr& r)
    {
	sCreated[info.hash_code()][rids] = r;
    }
    
    RangePtr RangeFactoryBase::getRegistered(const TypeInfo& info, const RangePtr& r)
    {
	CXZ_ASSERT(r != nullptr, "got range ptr to null");
	auto& rx = sCreated[info.hash_code()][r->key()];
	if(rx == nullptr){
	    rx = r;
	}
	return rx;
    }

    /******************
     *   RangeBase    *
     ******************/

    RangeBase::RangeBase()
    {
	mId = mkUuid();
    }
    
    RangePtr RangeBase::sub(SizeT num) const
    {
	return nullptr;
    }

    MArray<RangePtr> RangeBase::sub() const
    {
	return MArray<RangePtr>();
    }

    bool RangeBase::operator==(const RangeBase& in) const
    {
	return this == &in;
    }
    
    bool RangeBase::operator!=(const RangeBase& in) const
    {
	return this != &in;
    }
    
    Uuid RangeBase::id() const
    {
        //return reinterpret_cast<PtrId>(this);
	return mId;
    }

    DIndex RangeBase::begin() const
    {
	return this->index(0);
    }
    
    DIndex RangeBase::end() const
    {
	return this->index(this->size());
    }

    /****************************
     *   Non-member functions   *
     ****************************/

    RangePack::operator RangePtr() const
    {
	return YRangeFactory(mRs).create();
    }
    
    RangePack operator*(const RangePtr& a, const RangePtr& b)
    {
	CXZ_ASSERT(a != nullptr, "first operand not initialized");
	CXZ_ASSERT(b != nullptr, "second operand not initialized");
	
	return RangePack { Vector<RangePtr> { a, b } };
	//return YRangeFactory(rvec).create();
    }

    RangePack operator*(const RangePtr& a, const RangePack& b)
    {
	CXZ_ASSERT(a != nullptr, "first operand not initialized");

	RangePack o { { a } };
	o.mRs.insert(o.mRs.end(), b.mRs.begin(), b.mRs.end());
	return o;
    }

    RangePack operator*(const RangePack& a, const RangePtr& b)
    {
	CXZ_ASSERT(b != nullptr, "first operand not initialized");

	RangePack o = a;
	o.mRs.push_back(b);
	return o;
    }

    RangePack operator*(const RangePack& a, const RangePack& b)
    {
	RangePack o = a;
	o.mRs.insert(o.mRs.end(), b.mRs.begin(), b.mRs.end());
	return o;
    }

    RangePtr getSub(const RangePtr& r, SizeT num)
    {
	RangePtr o = r->sub(num);
	CXZ_ASSERT(o != nullptr, "try to access empty subrange at position " << num);
	return o;
    }

    MArray<RangePtr> getSub(const RangePtr& r)
    {
	MArray<RangePtr> o = r->sub();
	CXZ_ASSERT(o.range() != nullptr, "try to access subrange of single range");
	return o;
    }

} // end namespace CNORXZ
