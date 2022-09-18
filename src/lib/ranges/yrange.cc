
#include "ranges/ranges.h"

namespace CNORXZ
{
    /***************
     *   YIndex    *
     ***************/

    YIndex::YIndex(const RangePtr& range, SizeT pos) :
	IndexInterface<YIndex,DType>(pos),
	mRangePtr(rangeCast<YRange>(range)), mIs(mRangePtr->dim()),
	mBlockSizes(mRangePtr->dim())
    {
	assert(0);
	// init ...!!!
    }

    YIndex::YIndex(const RangePtr& range, const Vector<XIndexPtr>& is, SizeT pos) :
	IndexInterface<YIndex,DType>(pos),
	mRangePtr(rangeCast<YRange>(range)), mIs(is),
	mBlockSizes(mRangePtr->dim())
    {
	CXZ_ASSERT(mIs.size() == mRangePtr->dim(), "obtained wrong number of indices");
	assert(0);
	// init ...!!!
    }

    YIndex& YIndex::sync()
    {
	assert(0);
	return *this;
    }
    
    YIndex& YIndex::operator=(SizeT pos)
    {
	IB::mPos = pos;
	assert(0);
	// sub inds... (LAZY!!!) !!!
	return *this;
    }
    
    YIndex& YIndex::operator++()
    {
	assert(0);
	// increment sub inds (LAZY!!!) !!!
	++mPos;
	return *this;
    }
    
    YIndex& YIndex::operator--()
    {
	assert(0);
	// decrement sub inds (LAZY!!!) !!!
	--mPos;
	return *this;
    }
    
    YIndex YIndex::operator+(Int n) const
    {
	assert(0);
	// sub inds !!!
	return YIndex(mRangePtr, IB::mPos + n);
    }
    
    YIndex YIndex::operator-(Int n) const
    {
	assert(0);
	// sub inds !!!
	return YIndex(mRangePtr, IB::mPos - n);
    }
    
    YIndex& YIndex::operator+=(Int n)
    {
	assert(0);
	// sub inds !!!
	IB::mPos += n;
	return *this;
    }
    
    YIndex& YIndex::operator-=(Int n)
    {
	assert(0);
	// sub inds !!!
	IB::mPos -= n;
	return *this;
    }

    DType YIndex::operator*() const
    {
	assert(0);
	return DType();
    }
    
    DType YIndex::operator->() const
    {
	assert(0);
	return DType();
    }
	
    Int YIndex::pp(PtrId idxPtrNum)
    {
	assert(0);
	// sub inds !!!
	return 0;
    }
    
    Int YIndex::mm(PtrId idxPtrNum)
    {
	assert(0);
	// sub inds !!!
	return 0;
    }
    
    SizeT YIndex::dim() const
    {
	return mRangePtr->dim();
    }

    Sptr<YRange> YIndex::range() const
    {
	return mRangePtr;
    }
    
    SizeT YIndex::getStepSize(PtrId iptr) const
    {
	assert(0);
	// sub inds !!!
	return 0;
    }

    Int YIndex::getOffset(PtrId iptr) const
    {
	assert(0);
	// sub inds !!!
	return 0;
    }

    String YIndex::stringMeta() const
    {
	String out = "[";
	auto it = mIs.begin();
	for(; it != mIs.end()-1; ++it){
	    out += (*it)->stringMeta() + ",";
	}
	out += (*it)->stringMeta() + "]";
	return out;
    }
   
    DType YIndex::meta() const
    {
	Vector<DType> v(mIs.size());
	std::transform(mIs.begin(), mIs.end(), v.begin(), [](auto& x) { return x->meta(); });
	return DType(v);
    }
    
    YIndex& YIndex::at(const DType& meta)
    {
	auto& v = std::any_cast<const Vector<DType>&>(meta.get());
	assert(v.size() == mIs.size());
	for(SizeT i = 0; i != mIs.size(); ++i){
	    mIs[i]->at(v[i]);
	}
	return *this;
    }
    /*
    DExpr YIndex::ifor(SizeT step, DExpr ex) const
    {
	assert(0);
	return DExpr();
    }
    
    DExpr YIndex::iforh(SizeT step, DExpr ex) const
    {
	assert(0);
	return DExpr();
    }
    */
    
    /**********************
     *   YRangeFactory    *
     **********************/

    YRangeFactory::YRangeFactory(const Vector<RangePtr>& rvec) :
	mRVec(rvec) {}
    
    YRangeFactory::YRangeFactory(Vector<RangePtr>&& rvec) :
	mRVec(std::forward<Vector<RangePtr>>(rvec)) {}
    
    YRangeFactory::YRangeFactory(const Vector<RangePtr>& rvec, const RangePtr& ref) :
	mRVec(rvec), mRef(ref) {}
    
    YRangeFactory::YRangeFactory(Vector<RangePtr>&& rvec, const RangePtr& ref) :
	mRVec(std::forward<Vector<RangePtr>>(rvec)), mRef(ref) {}
    
    void YRangeFactory::make()
    {
	Vector<PtrId> key;
	std::transform(mRVec.begin(), mRVec.end(), key.begin(),
		       [&](const RangePtr& r) { return r->id(); } );
	mProd = this->fromCreated(typeid(YRange), key);
	if(mProd == nullptr){
	    mProd = std::shared_ptr<YRange>
		( new YRange( std::move(mRVec) ) );
	    this->addToCreated(typeid(YRange), key, mProd);
	}
    }

    /***************
     *   YRange    *
     ***************/

    SizeT YRange::size() const
    {
	SizeT out = 1;
	for(auto& r: mRVec){
	    out *= r->size();
	}
	return out;
    }
    
    SizeT YRange::dim() const
    {
	return mRVec.size();
    }
    
    String YRange::stringMeta(SizeT pos) const
    {
	String out = "[";
	for(auto rit = mRVec.end()-1;;--rit){
	    const SizeT cursize = (*rit)->size();
	    const SizeT curpos = pos % cursize;
	    out += (*rit)->stringMeta(curpos);
	    pos -= curpos;
	    pos /= cursize;
	    if(rit == mRVec.begin()){
		out += "]";
		break;
	    }
	    out += ",";
	}
	return out;
    }
    
    const TypeInfo& YRange::type() const
    {
	return typeid(YRange);
    }

    const TypeInfo& YRange::metaType() const
    {
	return typeid(DType);
    }

    YRange::YRange(const Vector<RangePtr>& rvec) : mRVec(rvec) {}
    
    YRange::YRange(Vector<RangePtr>&& rvec) : mRVec(std::forward<Vector<RangePtr>>(rvec)) {}

}
