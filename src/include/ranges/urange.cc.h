
#ifndef __cxz_urange_cc_h__
#define __cxz_urange_cc_h__

#include <functional>
#include <algorithm>

#include "urange.h"

namespace CNORXZ
{
    /*****************
     *    UIndex     *	     
     *****************/

    namespace
    {
	template <typename MetaType>
	Sptr<URange<MetaType>> rc(const RangePtr r)
	{
	    if(typeid(URange<MetaType>) == r.type()){
		return std::dynamic_pointer_cast<URange<MetaType>>(r)
	    }
	    else {
		return rangeCast<URange<MetaType>>(r);
	    }
	}
    }
    
    template <typename MetaType>
    UIndex<MetaType>::UIndex(const RangePtr& range) :
	IndexInterface<UIndex<MetaType>,MetaType>(0),
	mRange(rc<MetaType>(range)),
	mMetaPtr(&get(0))
    {}

    template <typename MetaType>
    UIndex<MetaType>& UIndex<MetaType>::operator=(size_t pos)
    {
	IB::mPos = pos;
	return *this;
    }

    template <typename MetaType>
    UIndex<MetaType>& UIndex<MetaType>::operator++()
    {
	++IB::mPos;
	return *this;
    }

    template <typename MetaType>
    UIndex<MetaType>& UIndex<MetaType>::operator--()
    {
	--IB::mPos;
	return *this;
    }

    template <typename MetaType>
    int UIndex<MetaType>::pp(std::intptr_t idxPtrNum)
    {
	++(*this);
	return 1;
    }

    template <typename MetaType>
    int UIndex<MetaType>::mm(std::intptr_t idxPtrNum)
    {
	--(*this);
	return 1;
    }

    template <typename MetaType>
    String UIndex<MetaType>::stringMeta() const
    {
	return toString(this->meta());
    }
    
    template <typename MetaType>
    MetaType& UIndex<MetaType>::meta() const
    {
	return mSpace[IB::mPos];
    }
    
    template <typename MetaType>
    UIndex<MetaType>& UIndex<MetaType>::at(const MetaType& metaPos)
    {
	(*this) = mRangePtr->getMeta(metaPos);
	return *this;
    }

    template <typename MetaType>
    size_t UIndex<MetaType>::dim() // = 1
    {
	return 1;
    }

    template <typename MetaType>
    Sptr<URange<MetaType>> UIndex<MetaType>::range()
    {
	return mRangePtr;
    }

    template <typename MetaType>
    size_t UIndex<MetaType>::getStepSize(size_t n)
    {
	return 1;
    }

    template <typename MetaType>
    template <class Expr>
    auto UIndex<MetaType>::ifor(size_t step, Expr ex) const
	-> For<URange<MetaType>,Expr>
    {
	return For<UIndex<MetaType>,Expr>(this, step, ex);
    }

    template <typename MetaType>
    template <class Expr>
    auto UIndex<MetaType>::iforh(size_t step, Expr ex) const
	-> For<URange<MetaType>,Expr,ForType::HIDDEN>
    {
	return For<UIndex<MetaType>,Expr,ForType::HIDDEN>(this, step, ex);
    }

    template <typename MetaType>
    template <class Expr>
    auto UIndex<MetaType>::pifor(size_t step, Expr ex) const
	-> PFor<URange<MetaType>,Expr>
    {
	return PFor<UIndex<MetaType>,Expr>(this, step, ex);
    }

    
    /********************
     *   SingleRange    *
     ********************/

    template <typename MetaType>
    URangeFactory<MetaType>::URangeFactory(const Vector<MetaType>& space) :
	mSpace(space) {}

    template <typename MetaType>
    URangeFactory<MetaType>::URangeFactory(Vector<MetaType>&& space) :
	mSpace(space) {}

    template <typename MetaType>
    URangeFactory<MetaType>::URangeFactory(const Vector<MetaType>& space, const RangePtr& ref) :
	mSpace(space), mRef(ref) {}

    template <typename MetaType>
    URangeFactory<MetaType>::URangeFactory(Vector<MetaType>&& space, const RangePtr& ref) :
	mSpace(space), mRef(ref) {}

    template <typename MetaType>
    void URangeFactory<MetaType>::make()
    {
	if(mRef != nullptr) {
	    mProd = this->fromCreated[typeid(oType)][mRef->id()];
	}
	if(mProd == nullptr){
	    RangePtr key = mProd = std::shared_ptr<oType>
		( new URange<MetaType>( std::move(mSpace) ) );
	    if(mRef != nullptr) { key = mRef->id(); }
	    this->addToCreated(typeid(oType), { key }, mProd);
	}
    }
    
    /********************
     *   SingleRange    *
     ********************/
    
    template <typename MetaType>
    URange<MetaType>::URange(const Vector<MetaType>& space) :
	RangeInterface<URange<MetaType>>(),
	mSpace(space)
    {
	std::sort(mSpace.begin(), mSpace.end(), std::less<MetaType>());
	auto itdupl = std::adjacent_find(mSpace.begin(), mSpace.end());
	CXZ_ASSERT(itdupl == mSpace.end(), "found duplicate: " << *itdupl);
    }

    template <typename MetaType>
    URange<MetaType>::URange(Vector<MetaType>&& space) :
	RangeInterface<URange<MetaType>>(),
	mSpace(space)
    {
	std::sort(mSpace.begin(), mSpace.end(), std::less<MetaType>());
	auto itdupl = std::adjacent_find(mSpace.begin(), mSpace.end());
	CXZ_ASSERT(itdupl == mSpace.end(), "found duplicate: " << *itdupl);
    }

        
    template <typename MetaType>
    const MetaType& URange<MetaType>::get(size_t pos) const
    {
	return mSpace[pos];
    }

    template <typename MetaType>
    SizeT URange<MetaType>::getMeta(const MetaType& meta) const
    {
	auto b = mSpace.begin();
	auto e = mSpace.end();
	return std::lower_bound(b, e, meta, std::less<MetaType>()) - b;
    }

    template <typename MetaType>
    SizeT URange<MetaType>::size() const
    {
	return mSpace.size();
    }

    template <typename MetaType>
    SizeT URange<MetaType>::dim() const
    {
	return 1;
    }

    template <typename MetaType>
    String URange<MetaType>::stringMeta(SizeT pos) const
    {
	return toString(this->get(pos));
    }
    
    template <typename MetaType>
    typename URange<MetaType>::IndexType URange<MetaType>::begin() const
    {
	UIndex<MetaType> i( std::dynamic_pointer_cast<URange<MetaType> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    template <typename MetaType>
    typename URange<MetaType>::IndexType URange<MetaType>::end() const
    {
	UIndex<MetaType> i( std::dynamic_pointer_cast<URange<MetaType> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = this->size();
	return i;
    }

    /*******************
     *   Range Casts   *
     *******************/

    template <typename MetaType>
    Sptr<URange<MetaType>> rangeCast<URange<MetaType>>(const RangePtr& r)
    {
	CXZ_ERROR("to be implemented...");
	return nullptr;
    }
}

#endif
