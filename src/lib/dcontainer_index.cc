
#include "dcontainer_index.h"

namespace CNORXZ
{
    YIndex::YIndex(const RangePtr& range) :
	IndexInterface<YIndex,DType>(range, 0)
    {
	//...!!!
    }
					    
    YIndex::YIndex(const RangePtr& range, const std::vector<XIndexPtr>& is) :
	IndexInterface<YIndex,DType>(range, 0)
    {
	//...!!!
    }

    YIndex& YIndex::operator=(size_t pos)
    {
	IB::mPos = pos;
	// sub inds...!!!
	return *this;
    }
    
    YIndex& YIndex::operator++()
    {
	if(mExternalControl) this->sync();
	// increment sub inds !!!
	++mPos;
	return *this;
    }

    YIndex& YIndex::operator--()
    {
	if(mExternalControl) this->sync();
	// decrement sub inds !!!
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
	return mIs.size();
    }

    size_t YIndex::getStepSize(size_t n) const
    {

    }

    std::string YIndex::stringMeta() const
    {
	std::string out = "[";
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
	vector<DType> v(mIs.size());
	std::transform(mIs.begin(), mIs.end(), v.begin(), [](auto& x) { return x->meta() });
	return DType(v);
    }

    YIndex& YIndex::at(const DType& meta)
    {
	auto& v = std::any_cast<const vector<DType>&>(meta.get());
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
    
} // namespace CNORXZ
