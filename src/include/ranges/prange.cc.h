
#ifndef __cxz_prange_cc_h__
#define __cxz_prange_cc_h__

#include "prange.h"

namespace CNORXZ
{
    /**************
     *   PIndex   *
     **************/
    
    template <class IndexT>
    PIndex<IndexT>::PIndex(const RangePtr& range, SizeT pos) :
	IndexInterface<PIndex<IndexT>,typename IndexT::MetaType>(pos),
	mRangePtr(rangeCast<RangeType>(range)),
	mOrig(std::make_shared<IndexT>(mRangePtr->orig(),mRangePtr->parts()[pos]))
    {}
	
    template <class IndexT>
    PIndex<IndexT>& PIndex<IndexT>::operator=(SizeT lexpos)
    {
	IB::mPos = lexpos;
	*mOrig = mRangePtr->parts()[IB::mPos];
	return *this;
    }
    
    template <class IndexT>
    PIndex<IndexT>& PIndex<IndexT>::operator++()
    {
	++IB::mPos;
	*mOrig = mRangePtr->parts()[IB::mPos];
	return *this;
    }
    
    template <class IndexT>
    PIndex<IndexT>& PIndex<IndexT>::operator--()
    {
	--IB::mPos;
	*mOrig = mRangePtr->parts()[IB::mPos];
	return *this;
    }
    
    template <class IndexT>
    PIndex<IndexT> PIndex<IndexT>::operator+(Int n) const
    {
	return PIndex(mRangePtr, IB::mPos + n);
    }
    
    template <class IndexT>
    PIndex<IndexT> PIndex<IndexT>::operator-(Int n) const
    {
	return PIndex(mRangePtr, IB::mPos - n);
    }
    
    template <class IndexT>
    PIndex<IndexT>& PIndex<IndexT>::operator+=(Int n)
    {
	IB::mPos += n;
	*mOrig = mRangePtr->parts()[IB::mPos];
	return *this;
    }
    
    template <class IndexT>
    PIndex<IndexT>& PIndex<IndexT>::operator-=(Int n)
    {
	IB::mPos -= n;
	*mOrig = mRangePtr->parts()[IB::mPos];
	return *this;
    }

    template <class IndexT>
    SizeT PIndex<IndexT>::lex() const
    {
	return IB::mPos;
    }
    
    template <class IndexT>
    UPos PIndex<IndexT>::pmax() const
    {
	return UPos(mRangePtr->size());
    }
    
    template <class IndexT>
    UPos PIndex<IndexT>::lmax() const
    {
	return UPos(mRangePtr->size());
    }
    
    template <class IndexT>
    IndexId<0> PIndex<IndexT>::id() const
    {
	return IndexId<0>(this->ptrId());
    }
	
    template <class IndexT>
    decltype(auto) PIndex<IndexT>::operator*() const
    {
	return **mOrig;
    }
	
    template <class IndexT>
    SizeT PIndex<IndexT>::dim() const
    {
	return 1;
    }
    
    template <class IndexT>
    Sptr<typename PIndex<IndexT>::RangeType> PIndex<IndexT>::range() const
    {
	return mRangePtr;
    }

    template <class IndexT>
    template <SizeT I>
    UPos PIndex<IndexT>::stepSize(const IndexId<I>& id) const
    {
	if(id == this->id()){
	    return UPos(1);
	}
	else {
	    return mOrig->stepSize(id);
	}
    }

    template <class IndexT>
    RangePtr PIndex<IndexT>::prange(const PIndex<IndexT>& end) const
    {
	CXZ_ASSERT(end > *this, "got end index position smaller than begin index position");
	auto oi = *orig();
	auto oend = *end.orig();
	const SizeT beginPos = oi.lex();
	Vector<SizeT> parts(oend.lex()-beginPos);
	for(auto i = oi; i != oend; ++i){
	    parts[i.lex()-beginPos] = i.lex();
	}
	return CNORXZ::prange(mRangePtr->orig(), parts);
    }

    template <class IndexT>
    decltype(auto) PIndex<IndexT>::deepFormat() const
    {
	return mOrig->deepFormat();
    }

    template <class IndexT>
    String PIndex<IndexT>::stringMeta() const
    {
	return mOrig->stringMeta();
    }
    
    template <class IndexT>
    decltype(auto) PIndex<IndexT>::meta() const
    {
	return mOrig->meta();
    }
    
    template <class IndexT>
    PIndex<IndexT>& PIndex<IndexT>::at(const MetaType& metaPos)
    {
	mOrig->at(metaPos);
	mkPos();
	return *this;
    }
    
    template <class IndexT>
    decltype(auto) PIndex<IndexT>::xpr(const Sptr<PIndex<IndexT>>& _this) const
    {
	CXZ_ERROR("implement!!!");
	//return poperation( _this, mOrig, mRangePtr->parts(), mOrig->xpr(mOrig) );
	return mOrig->xpr(mOrig);
    }

    template <class IndexT>
    template <class I>
    decltype(auto) PIndex<IndexT>::reformat(const Sptr<I>& ind) const
    {
	return ind;
    }

    template <class IndexT>
    template <class I>
    decltype(auto) PIndex<IndexT>::slice(const Sptr<I>& ind) const
    {
	if(ind != nullptr){
	    if(ind->dim() != 0){
		return Sptr<PIndex<IndexT>>();
	    }
	}
	return std::make_shared<PIndex<IndexT>>(*this);
    }

    template <class IndexT>
    template <class Xpr, class F>
    decltype(auto) PIndex<IndexT>::ifor(const Xpr& xpr, F&& f) const
    {
	return PFor<0,0,Xpr,F>(this->lmax().val(), this->id(), mOrig->id(),
			       mRangePtr->parts().data(), xpr, std::forward<F>(f));
    }

    template <class IndexT>
    PIndex<IndexT>& PIndex<IndexT>::operator()()
    {
	mkPos();
	return *this;
    }

    template <class IndexT>
    PIndex<IndexT>& PIndex<IndexT>::operator()(const Sptr<IndexT>& i)
    {
	mOrig = i;
	mkPos();
	return *this;
    }

    template <class IndexT>
    const Sptr<IndexT>& PIndex<IndexT>::orig() const
    {
	return mOrig;
    }

    /************************
     *   PIndex (private)   *
     ************************/

    template <class IndexT>
    void PIndex<IndexT>::mkPos()
    {
	const SizeT opos = mOrig->lex();
	IB::mPos = 0;
	for(const auto& x: mRangePtr->parts()){
	    if(x == opos){
		return;
	    }
	    ++IB::mPos;
	}
	CXZ_ERROR("meta position '" << toString(mOrig->meta()) << "' not part of range");
    }

    /***************************
     *   PIndex (non-member)   *
     ***************************/

    template <class I, class I1>
    decltype(auto) operator*(const Sptr<PIndex<I>>& a, const Sptr<I1>& b)
    {
	return iptrMul(a, b);
    }

    /*********************
     *   PRangeFactory   *
     *********************/

    template <class RangeT>
    PRangeFactory<RangeT>::PRangeFactory(const Sptr<RangeT>& range, const Vector<SizeT>& _parts) :
	mRange(range), mParts(_parts)
    {}

    template <class RangeT>
    PRangeFactory<RangeT>::PRangeFactory(const RangePtr& range, const Vector<SizeT>& _parts) :
	mRange(rangeCast<RangeT>(range)), mParts(_parts)
    {}

    template <class RangeT>
    void PRangeFactory<RangeT>::make()
    {
	const Vector<Uuid> key = { mRange->id() };
	const auto& info = typeid(PRange<RangeT>);
	mProd = this->fromCreated(info, key);
	if(mProd == nullptr) {
	    mProd = std::shared_ptr<PRange<RangeT>>( new PRange<RangeT>(mRange, mParts) );
	    this->addToCreated(info, key, mProd);
	}
    }

    /**************
     *   PRange   *
     **************/

    template <class RangeT>
    SizeT PRange<RangeT>::size() const
    {
	return mParts.size();
    }
    
    template <class RangeT>
    SizeT PRange<RangeT>::dim() const
    {
	return 1;
    }
    
    template <class RangeT>
    String PRange<RangeT>::stringMeta(SizeT pos) const
    {
	return mRange->stringMeta( mParts[pos] );
    }
    
    template <class RangeT>
    const TypeInfo& PRange<RangeT>::type() const
    {
	return typeid(PRange<RangeT>);
    }
    
    template <class RangeT>
    const TypeInfo& PRange<RangeT>::metaType() const
    {
	return mRange->metaType();
    }
    
    template <class RangeT>
    RangePtr PRange<RangeT>::extend(const RangePtr& r) const
    {
	CXZ_ERROR("implement!!!");
	return nullptr;
    }

    template <class RangeT>
    Sptr<RangeT> PRange<RangeT>::orig() const
    {
	return mRange;
    }
    
    template <class RangeT>
    const Vector<SizeT>& PRange<RangeT>::parts() const
    {
	return mParts;
    }
    
    template <class RangeT>
    RangePtr PRange<RangeT>::derive() const
    {
	Vector<MetaType> meta(this->size());
	auto i = mRange->begin();
	for(const auto& p: mParts){
	    meta = *(i = p);
	}
	return URangeFactory<MetaType>( meta ).create();
    }

    
    /************************
     *   PRange (private)   *
     ************************/

    template <class RangeT>
    PRange<RangeT>::PRange(const Sptr<RangeT>& range, const Vector<SizeT>& _parts) :
	mRange(range), mParts(_parts)
    {}

    template <class RangeT>
    Vector<Uuid> PRange<RangeT>::key() const
    {
	return Vector<Uuid> { mRange->id() };
    }

    /****************************
     *   non-member functions   *
     ****************************/

    template <class RangeT>
    RangePtr prange(const Sptr<RangeT>& range, const Vector<SizeT>& parts)
    {
	return PRangeFactory<RangeT>(range,parts).create();
    }

}

#endif
