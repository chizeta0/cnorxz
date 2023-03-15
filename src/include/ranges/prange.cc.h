
#ifndef __cxz_prange_cc_h__
#define __cxz_prange_cc_h__

#include "prange.h"

namespace CNORXZ
{
    /**************
     *   PIndex   *
     **************/
    
    template <class Index>
    PIndex<Index>::PIndex(const RangePtr& range, SizeT pos) :
	IndexInterface<Index,typename Index::MetaType>(pos),
	mRangePtr(rangeCast<RangeType>(range)),
	mOrig(mRangePtr->orig(),mRangePtr->parts()[pos])
    {}
	
    template <class Index>
    PIndex<Index>& PIndex<Index>::operator=(SizeT lexpos)
    {
	IB::mPos = lexpos;
	*mOrig = mRangePtr->parts()[IB::mPos];
	return *this;
    }
    
    template <class Index>
    PIndex<Index>& PIndex<Index>::operator++()
    {
	++IB::mPos;
	*mOrig = mRangePtr->parts()[IB::mPos];
	return *this;
    }
    
    template <class Index>
    PIndex<Index>& PIndex<Index>::operator--()
    {
	--IB::mPos;
	*mOrig = mRangePtr->parts()[IB::mPos];
	return *this;
    }
    
    template <class Index>
    PIndex<Index> PIndex<Index>::operator+(Int n) const
    {
	return PIndex(mRangePtr, IB::mPos + n);
    }
    
    template <class Index>
    PIndex<Index> PIndex<Index>::operator-(Int n) const
    {
	return PIndex(mRangePtr, IB::mPos - n);
    }
    
    template <class Index>
    PIndex<Index>& PIndex<Index>::operator+=(Int n)
    {
	IB::mPos += n;
	*mOrig = mRangePtr->parts()[IB::mPos];
	return *this;
    }
    
    template <class Index>
    PIndex<Index>& PIndex<Index>::operator-=(Int n)
    {
	IB::mPos -= n;
	*mOrig = mRangePtr->parts()[IB::mPos];
	return *this;
    }

    template <class Index>
    SizeT PIndex<Index>::lex() const
    {
	return IB::mPos;
    }
    
    template <class Index>
    UPos PIndex<Index>::pmax() const
    {
	return UPos(mRangePtr->size());
    }
    
    template <class Index>
    UPos PIndex<Index>::lmax() const
    {
	return UPos(mRangePtr->size());
    }
    
    template <class Index>
    IndexId<0> PIndex<Index>::id() const
    {
	return IndexId<0>(this->ptrId());
    }
	
    template <class Index>
    const typename PIndex<Index>::MetaType& PIndex<Index>::operator*() const
    {
	return **mOrig;
    }
	
    template <class Index>
    SizeT PIndex<Index>::dim() const
    {
	return 1;
    }
    
    template <class Index>
    Sptr<typename PIndex<Index>::RangeType> PIndex<Index>::range() const
    {
	return mRangePtr;
    }

    template <class Index>
    template <SizeT I>
    UPos PIndex<Index>::stepSize(const IndexId<I>& id) const
    {
	if(id == this->id()){
	    return UPos(1);
	}
	else {
	    return mOrig->stepSize(id);
	}
    }

    template <class Index>
    String PIndex<Index>::stringMeta() const
    {
	return mOrig->stringMeta();
    }
    
    template <class Index>
    const typename PIndex<Index>::MetaType& PIndex<Index>::meta() const
    {
	return mOrig->meta();
    }
    
    template <class Index>
    PIndex<Index>& PIndex<Index>::at(const MetaType& metaPos)
    {
	mOrig->at(metaPos);
	mkPos();
	return *this;
    }
    
    template <class Index>
    decltype(auto) PIndex<Index>::xpr(const Sptr<PIndex<Index>>& _this) const
    {
	return poperation( mOrig->xpr(mOrig), mRangePtr->parts(), _this );
    }

    template <class Index>
    template <class I>
    decltype(auto) PIndex<Index>::format(const Sptr<I>& ind) const
    {
	return ind;
    }

    template <class Index>
    template <class I>
    decltype(auto) PIndex<Index>::slice(const Sptr<I>& ind) const
    {
	if(ind != nullptr){
	    if(ind->dim() != 0){
		return Sptr<PIndex<Index>>();
	    }
	}
	return std::make_shared<PIndex<Index>>(*this);
    }

    template <class Index>
    template <class Xpr, class F>
    decltype(auto) PIndex<Index>::ifor(const Xpr& xpr, F&& f) const
    {
	return PFor<0,0,Xpr,F>(this->lmax().val(), this->id(), mOrig->id(), xpr, std::forward<F>(f));
    }

    template <class Index>
    PIndex<Index>& PIndex<Index>::operator()()
    {
	mkPos();
	return *this;
    }

    template <class Index>
    PIndex<Index>& PIndex<Index>::operator()(const Sptr<Index>& i)
    {
	mOrig = i;
	mkPos();
	return *this;
    }

    /************************
     *   PIndex (private)   *
     ************************/

    template <class Index>
    void PIndex<Index>::mkPos()
    {
	const SizeT opos = mOrig->lex();
	IB::mPos = 0;
	for(const auto& x: mRangePtr->parts()){
	    if(x == opos){
		return;
	    }
	    ++IB::mPos;
	}
	CXZ_ERROR("meta position '" << mOrig->meta() << "' not part of range");
    }

}

#endif
