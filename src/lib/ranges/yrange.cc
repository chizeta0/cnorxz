
#include "ranges/ranges.h"

namespace CNORXZ
{
    YIndex::YIndex(const RangePtr& range) :
	mRange(range), mIs(mRange->dim()),
	mBlockSizes(mRange->dim()), mExternalControl(false)
    {
	assert(0);
	// init ...!!!
    }

    YIndex::YIndex(const RangePtr& range, const Vector<XIndexPtr>& is) :
	mRange(range), mIs(is),
	mBlockSizes(mRange->dim()), mExternalControl(false)
    {
	CXZ_ASSERT(mIs.size() == mRange->dim(), "obtained wrong number of indices");
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
	// sub inds... (LAZY!!!) !!!
	return *this;
    }
    
    YIndex& YIndex::operator++()
    {
	if(mExternalControl) this->sync();
	// increment sub inds (LAZY!!!) !!!
	++mPos;
	return *this;
    }
    
    YIndex& YIndex::operator--()
    {
	if(mExternalControl) this->sync();
	// decrement sub inds (LAZY!!!) !!!
	--mPos;
	return *this;
    }
    
    Int YIndex::pp(PtrId idxPtrNum)
    {
	assert(0);
	return 0;
    }
    
    Int YIndex::mm(PtrId idxPtrNum)
    {
	assert(0);
	return 0;
    }
    
    SizeT YIndex::dim() const
    {
	return mRange->dim();
    }
    
    SizeT YIndex::getStepSize(SizeT n) const
    {
	assert(0);
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
	//this->sync();
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

}
