
#ifndef __cxz_range_cc_h__
#define __cxz_range_cc_h__

#include "srange.h"

namespace CNORXZ
{
    /**************
     *   SIndex   *
     **************/

    template <typename MetaT, SizeT S>
    SIndex<MetaT,S>::SIndex(const RangePtr& range, SizeT pos) :
	IndexInterface<SIndex<MetaT,S>,MetaT>(pos),
	mRangePtr(rangeCast<RangeType>(range)),
	mMetaPtr(&mRangePtr->get(0))
    {
	CXZ_ASSERT(mRangePtr->size() == S, "got range of size " << mRangePtr->size()
		   << ", expected " << S);
    }	

    template <typename MetaT, SizeT S>
    SIndex<MetaT,S>& SIndex<MetaT,S>::operator=(SizeT lexpos)
    {
	IB::mPos = lexpos;
	return *this;
    }
	
    template <typename MetaT, SizeT S>
    SIndex<MetaT,S>& SIndex<MetaT,S>::operator++()
    {
	++IB::mPos;
	return *this;
    }

    template <typename MetaT, SizeT S>
    SIndex<MetaT,S>& SIndex<MetaT,S>::operator--()
    {
	--IB::mPos;
	return *this;
    }

    template <typename MetaT, SizeT S>
    SIndex<MetaT,S> SIndex<MetaT,S>::operator+(Int n) const
    {
	return SIndex(mRangePtr, IB::mPos + n);
    }

    template <typename MetaT, SizeT S>
    SIndex<MetaT,S> SIndex<MetaT,S>::operator-(Int n) const
    {
	return SIndex(mRangePtr, IB::mPos - n);
    }

    template <typename MetaT, SizeT S>
    SIndex<MetaT,S>& SIndex<MetaT,S>::operator+=(Int n)
    {
	IB::mPos += n;
	return *this;
    }

    template <typename MetaT, SizeT S>
    SIndex<MetaT,S>& SIndex<MetaT,S>::operator-=(Int n)
    {
	IB::mPos -= n;
	return *this;
    }

    template <typename MetaT, SizeT S>
    SizeT SIndex<MetaT,S>::lex() const
    {
	return IB::mPos;
    }

    template <typename MetaT, SizeT S>
    SPos<S> SIndex<MetaT,S>::pmax() const
    {
	return SPos<S>();
    }

    template <typename MetaT, SizeT S>
    SPos<S> SIndex<MetaT,S>::lmax() const
    {
	return SPos<S>();
    }

    template <typename MetaT, SizeT S>
    IndexId<0> SIndex<MetaT,S>::id() const
    {
	return IndexId<0>(this->ptrId());
    }
	
    template <typename MetaT, SizeT S>
    const MetaT& SIndex<MetaT,S>::operator*() const
    {
	return mMetaPtr[IB::mPos];
    }
	
    template <typename MetaT, SizeT S>
    SizeT SIndex<MetaT,S>::dim() const
    {
	return 1;
    }

    template <typename MetaT, SizeT S>
    Sptr<SRange<MetaT,S>> SIndex<MetaT,S>::range() const
    {
	return mRangePtr;
    }

    template <typename MetaT, SizeT S>
    template <SizeT I>
    UPos SIndex<MetaT,S>::stepSize(const IndexId<I>& id) const
    {
	return UPos(id == this->id() ? 1 : 0);
    }

    template <typename MetaT, SizeT S>
    String SIndex<MetaT,S>::stringMeta() const
    {
	return toString(this->meta());
    }

    template <typename MetaT, SizeT S>
    const MetaT& SIndex<MetaT,S>::meta() const
    {
	return mMetaPtr[IB::mPos];
    }

    template <typename MetaT, SizeT S>
    SIndex<MetaT,S>& SIndex<MetaT,S>::at(const MetaT& metaPos)
    {
	(*this) = mRangePtr->getMeta(metaPos);
	return *this;
    }

    template <typename MetaT, SizeT S>
    decltype(auto) SIndex<MetaT,S>::xpr(const Sptr<SIndex<MetaType,S>>& _this) const
    {
	return coproot(mMetaPtr,_this);
    }

    template <typename MetaT, SizeT S>
    SizeT SIndex<MetaT,S>::deepFormat() const
    {
	return 1;
    }
    /*
    template <typename MetaT, SizeT S>
    template <class Index>
    decltype(auto) SIndex<MetaT,S>::formatFrom(const Index& ind) const
    {
	return *this;
    }

    template <typename MetaT, SizeT S>
    template <class Index>
    decltype(auto) SIndex<MetaT,S>::slice(const Sptr<Index>& ind) const
    {
	if(ind != nullptr){
	    if(ind->dim() != 0) {
		return Sptr<SIndex<MetaType,S>>();
	    }
	}
	return std::make_shared<SIndex<MetaType,S>>(*this);
    }
    */
    template <typename MetaT, SizeT S>
    template <class Xpr, class F>
    decltype(auto) SIndex<MetaT,S>::ifor(const Xpr& xpr, F&& f) const
    {
	return SFor<S,0,Xpr,F>(this->id(), xpr, std::forward<F>(f));
    }

    template <typename MetaType, SizeT S, class I1>
    decltype(auto) operator*(const Sptr<SIndex<MetaType,S>>& a, const Sptr<I1>& b)
    {
	return iptrMul(a, b);
    }

    
    /*********************
     *   SRangeFactory   *
     *********************/

    template <typename MetaType, SizeT S>
    SRangeFactory<MetaType,S>::SRangeFactory(const Arr<MetaType,S>& space) :
	mSpace(space) {}

    template <typename MetaType, SizeT S>
    SRangeFactory<MetaType,S>::SRangeFactory(Arr<MetaType,S>&& space) :
	mSpace(std::forward<Arr<MetaType,S>>(space)) {}

    template <typename MetaType, SizeT S>
    SRangeFactory<MetaType,S>::SRangeFactory(const Arr<MetaType,S>& space, const RangePtr& ref) :
	mSpace(space), mRef(ref) {}

    template <typename MetaType, SizeT S>
    SRangeFactory<MetaType,S>::SRangeFactory(Arr<MetaType,S>&& space, const RangePtr& ref) : 
	mSpace(std::forward<Arr<MetaType,S>>(space)), mRef(ref) {}

    template <typename MetaType, SizeT S>
    void SRangeFactory<MetaType,S>::make()
    {
	const auto& info = typeid(SRange<MetaType,S>);
	if(mRef != nullptr) {
	    mProd = this->fromCreated(info, {mRef->id()});
	}
	if(mProd == nullptr){
	    RangePtr key = mProd = std::shared_ptr<SRange<MetaType,S>>
		( new SRange<MetaType,S>( std::move(mSpace) ) );
	    if(mRef != nullptr) { key = mRef; }
	    this->addToCreated(info, { key->id() }, mProd);
	}
    }

    /**************
     *   SRange   *
     **************/
    
    template <typename MetaType, SizeT S>
    SRange<MetaType,S>::SRange(const Arr<MetaType,S>& space) :
	RangeInterface<SRange<MetaType,S>>(),
	mSpace(space)
    {
	std::sort(mSpace.begin(), mSpace.end(), std::less<MetaType>());
	auto itdupl = std::adjacent_find(mSpace.begin(), mSpace.end());
	CXZ_ASSERT(itdupl == mSpace.end(), "found duplicate: " << *itdupl);
    }
    
    template <typename MetaType, SizeT S>
    SRange<MetaType,S>::SRange(Arr<MetaType,S>&& space) :
	RangeInterface<SRange<MetaType,S>>(),
	mSpace(std::forward<Arr<MetaType,S>>(space))
    {
	std::sort(mSpace.begin(), mSpace.end(), std::less<MetaType>());
	auto itdupl = std::adjacent_find(mSpace.begin(), mSpace.end());
	CXZ_ASSERT(itdupl == mSpace.end(), "found duplicate: " << *itdupl);
    }

    template <typename MetaType, SizeT S>
    SizeT SRange<MetaType,S>::size() const
    {
	return S;
    }
    
    template <typename MetaType, SizeT S>
    SizeT SRange<MetaType,S>::dim() const 
    {
	return 1;
    }

    template <typename MetaType, SizeT S>
    String SRange<MetaType,S>::stringMeta(SizeT pos) const 
    {
	return toString(mSpace[pos]);
    }

    template <typename MetaType, SizeT S>
    const TypeInfo& SRange<MetaType,S>::type() const 
    {
	return typeid(SRange<MetaType,S>);
    }

    template <typename MetaType, SizeT S>
    const TypeInfo& SRange<MetaType,S>::metaType() const 
    {
	return typeid(MetaType);
    }

    template <typename MetaType, SizeT S>
    RangePtr SRange<MetaType,S>::extend(const RangePtr& r) const 
    {
	// TODO: check for selected static sizes of SRange -> return SRange!!!
	auto rx = rangeCast<URange<MetaType>>(r);
	Vector<MetaType> space(mSpace.begin(), mSpace.end());
	space.insert(space.end(), rx->begin(), rx->end());
	return URangeFactory<MetaType>( space ).create();
    }

    template <typename MetaType, SizeT S>
    const MetaType& SRange<MetaType,S>::get(SizeT pos) const
    {
	return mSpace[pos];
    }

    template <typename MetaType, SizeT S>
    const MetaType* SRange<MetaType,S>::get() const
    {
	return mSpace.data();
    }

    template <typename MetaType, SizeT S>
    SizeT SRange<MetaType,S>::getMeta(const MetaType& metaPos) const
    {
	auto b = mSpace.begin();
	auto e = mSpace.end();
	return std::lower_bound(b, e, metaPos, std::less<MetaType>()) - b;
    }

    template <typename MetaType, SizeT S>
    Vector<Uuid> SRange<MetaType,S>::key() const 
    {
	return Vector<Uuid> { this->id() };
    }

    /*******************
     *   Range Casts   *
     *******************/

    template <typename MetaType, SizeT S>
    Sptr<SRange<MetaType,S>> RangeCast<SRange<MetaType,S>>::func(const RangePtr& r)
    {
	Sptr<URange<MetaType>> tmp;
	if(r->type() != typeid(URange<MetaType>)){
	    tmp = rangeCast<URange<MetaType>>(r);
	}
	else {
	    tmp = std::dynamic_pointer_cast<URange<MetaType>>(r);
	}
	CXZ_ASSERT(tmp->size() == S, "cannot cast range of size " << tmp->size()
		   << " into static range of size " << S);
	Arr<MetaType,S> space;
	std::copy(tmp->begin(), tmp->end(), space.begin());
	return std::dynamic_pointer_cast<SRange<MetaType,S>>
	    ( SRangeFactory<MetaType,S>(std::move(space)).create() );
    }
    
}

#endif
