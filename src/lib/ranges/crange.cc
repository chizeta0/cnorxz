
#include "ranges/ranges.h"

namespace CNORXZ
{
    /***************
     *   CIndex    *
     ***************/

    CIndex::CIndex(const RangePtr& range, SizeT pos) :
	IndexInterface<CIndex,SizeT>(pos), mRangePtr(rangeCast<RangeType>(range))
    {}
    
    CIndex& CIndex::operator=(SizeT pos)
    {
	IB::mPos = pos;
	return *this;
    }
    
    CIndex& CIndex::operator++()
    {
	++IB::mPos;
	return *this;
    }
    
    CIndex& CIndex::operator--()
    {
	--IB::mPos;
	return *this;
    }
    
    CIndex CIndex::operator+(Int n) const
    {
	return CIndex(mRangePtr, IB::mPos + n);
    }
    
    CIndex CIndex::operator-(Int n) const
    {
	return CIndex(mRangePtr, IB::mPos - n);
    }
    
    CIndex& CIndex::operator+=(Int n)
    {
	IB::mPos += n;
	return *this;
    }
    
    CIndex& CIndex::operator-=(Int n)
    {
	IB::mPos -= n;
	return *this;
    }

    SizeT CIndex::max() const
    {
	return mRangePtr->size();
    }
    
    IndexId<0> CIndex::id() const
    {
	return IndexId<0>(this->ptrId());
    }

    SizeT CIndex::operator*() const
    {
	return IB::mPos;
    }
    
    SizeT CIndex::operator->() const
    {
	return IB::mPos; // check if this makes sense!!!
    }
	
    Int CIndex::pp(PtrId idxPtrNum)
    {
	++(*this);
	return 1;
    }
    
    Int CIndex::mm(PtrId idxPtrNum)
    {
	--(*this);
	return 1;
    }

    SizeT CIndex::dim() const // = 1
    {
	return 1;
    }
    
    Sptr<CIndex::RangeType> CIndex::range() const
    {
	return mRangePtr;
    }
    
    UPos CIndex::stepSize(const IndexId<0>& id) const
    {
	return UPos(id == this->id() ? 1 : 0);
    }

    String CIndex::stringMeta() const
    {
	return toString(IB::mPos);
    }
    
    SizeT CIndex::meta() const
    {
	return IB::mPos;
    }
    
    CIndex& CIndex::at(const SizeT& metaPos)
    {
	IB::mPos = metaPos;
	return *this;
    }
    
    
    /**********************
     *   CRangeFactory    *
     **********************/

    CRangeFactory::CRangeFactory(SizeT size) :
	mSize(size) {}

    CRangeFactory::CRangeFactory(SizeT size, RangePtr ref) :
	mSize(size), mRef(ref) {}

    void CRangeFactory::make()
    {
	if(mRef != nullptr) {
	    mProd = this->fromCreated(typeid(CRange), {mRef->id()});
	}
	if(mProd == nullptr){
	    RangePtr key = mProd = std::shared_ptr<CRange>
		( new CRange( mSize ) );
	    if(mRef != nullptr) { key = mRef; }
	    this->addToCreated(typeid(CRange), { key->id() }, mProd);
	}
    }
    
    /***************
     *   CRange    *
     ***************/

    CRange::CRange(SizeT size) : mSize(size) {}
    
    SizeT CRange::get(SizeT pos) const
    {
	return pos;
    }
    
    SizeT CRange::getMeta(SizeT metaPos) const
    {
	return metaPos;
    }
    
    SizeT CRange::size() const
    {
	return mSize;
    }
    
    SizeT CRange::dim() const
    {
	return 1;
    }

    std::string CRange::stringMeta(SizeT pos) const
    {
	return toString(pos);
    }

    const TypeInfo& CRange::type() const
    {
	return typeid(CRange);
    }

    const TypeInfo& CRange::metaType() const
    {
	return typeid(SizeT);
    }

    /*******************
     *   Range Casts   *
     *******************/

    Sptr<CRange> RangeCast<CRange>::func(const RangePtr& r)
    {
	return std::dynamic_pointer_cast<CRange>( CRangeFactory(r->size(), r).create() );
    }
}

