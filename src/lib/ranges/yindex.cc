
#include "ranges/yindex.h"

namespace CNORXZ
{
    YIndex::YIndex(const RangePtr& range) :
	mIs(mRange.dim()), mBlockSizes(mRange.dim()), mExternalControl(false)
    {
	// init ...!!!
    }

    YIndex::YIndex(const RangePtr& range, const Vector<XIndexPtr>& is) :
	mIs(is), mBlockSizes(mRange.dim()), mExternalControl(false)
    {
	CXZ_ASSERT(mIs.size() == mRange.dim(), "obtained wrong number of indices");
	// init ...!!!
    }

    YIndex& YIndex::operator=(size_t pos)
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
    
    int YIndex::pp(std::intptr_t idxPtrNum)
    {
	
    }
    
    int YIndex::mm(std::intptr_t idxPtrNum)
    {

    }
    
    size_t YIndex::dim() const
    {
	return mRange->dim();
    }
    
    size_t YIndex::getStepSize(size_t n) const
    {
	
    }
    
    String YIndex::stringMeta() const
    {
	String out = "[";
	auto it = mIs.begin();
	for(; it != mIs.end()-1; ++it){
	    out += it->stringMeta() + ","
	}
	out += it->stringMeta() + "]"
	return out;
    }
    
    DType YIndex::meta() const
    {
	//this->sync();
	Vector<DType> v(mIs.size());
	std::transform(mIs.begin(), mIs.end(), v.begin(), [](auto& x) { return x->meta() });
	return DType(v);
    }
    
    YIndex& YIndex::at(const DType& meta)
    {
	auto& v = std::any_cast<const Vector<DType>&>(meta.get());
	assert(v.size() == mIs.size());
	for(size_t i = 0; i != mIs.size()){
	    mIs[i]->at(v[i]);
	}
	return *this;
    }
    
    DynamicExpression YIndex::ifor(size_t step, DynamicExpression ex) const
    {

    }
    
    DynamicExpression YIndex::iforh(size_t step, DynamicExpression ex) const
    {

    }

}
