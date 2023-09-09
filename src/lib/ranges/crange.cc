
#include "ranges/ranges.h"
#include "operation/operation.h"

namespace CNORXZ
{
    /***************
     *   CIndex    *
     ***************/

    CIndex::CIndex(const RangePtr& range, SizeT pos) :
	IndexInterface<CIndex,SizeT>(pos), mRangePtr(rangeCast<RangeType>(range))
    {}
    
    CIndex& CIndex::operator=(SizeT lexpos)
    {
	IB::mPos = lexpos;
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

    SizeT CIndex::lex() const
    {
	return IB::mPos;
    }
    
    UPos CIndex::lmax() const
    {
	return UPos(mRangePtr->size());
    }

    UPos CIndex::pmax() const
    {
	return UPos(mRangePtr->size());
    }

    IndexId<0> CIndex::id() const
    {
	return IndexId<0>(this->ptrId());
    }

    SizeT CIndex::operator*() const
    {
	return IB::mPos;
    }
    
    SizeT CIndex::dim() const // = 1
    {
	return 1;
    }
    
    Sptr<CIndex::RangeType> CIndex::range() const
    {
	return mRangePtr;
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

    COpRoot<SizeT,CIndex> CIndex::xpr(const Sptr<CIndex>& _this) const
    {
	// preliminary solution (TODO: implement xpr that simply returns PosT value):
	static Vector<SizeT> m;
	if(m.size() < _this->lmax().val()){
	    m.resize(_this->lmax().val());
	    for(SizeT i = 0; i != m.size(); ++i) {
		m[i] = i;
	    }
	}
	return coproot(m.data(), _this);
    }
    
    SizeT CIndex::deepFormat() const
    {
	return 1;
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

    RangePtr CRange::extend(const RangePtr& r) const
    {
	auto rx = rangeCast<CRange>(r);
	return CRangeFactory( this->size() + rx->size() ).create();
    }

    Vector<Uuid> CRange::key() const
    {
	return Vector<Uuid> { this->id() };
    }
    
    /*******************
     *   Range Casts   *
     *******************/

    Sptr<CRange> RangeCast<CRange>::func(const RangePtr& r)
    {
	return std::dynamic_pointer_cast<CRange>( CRangeFactory(r->size(), r).create() );
    }
}

