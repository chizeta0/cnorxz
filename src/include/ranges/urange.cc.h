
#ifndef __cxz_urange_cc_h__
#define __cxz_urange_cc_h__

#include <functional>
#include <algorithm>

#include "urange.h"
#include "xpr/for.h"

namespace CNORXZ
{
    /*****************
     *    UIndex     *	     
     *****************/

    template <typename MetaType>
    UIndex<MetaType>::UIndex(const RangePtr& range, SizeT pos) :
	IndexInterface<UIndex<MetaType>,MetaType>(pos),
	mRangePtr(rangeCast<RangeType>(range)),
	mMetaPtr(&mRangePtr->get(0))
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
    UIndex<MetaType> UIndex<MetaType>::operator+(Int n) const
    {
	return UIndex(mRangePtr, IB::mPos + n);
    }
    
    template <typename MetaType>
    UIndex<MetaType> UIndex<MetaType>::operator-(Int n) const
    {
	return UIndex(mRangePtr, IB::mPos - n);
    }
    
    template <typename MetaType>
    UIndex<MetaType>& UIndex<MetaType>::operator+=(Int n)
    {
	IB::mPos += n;
	return *this;
    }
    
    template <typename MetaType>
    UIndex<MetaType>& UIndex<MetaType>::operator-=(Int n)
    {
	IB::mPos -= n;
	return *this;
    }

    template <typename MetaType>
    SizeT UIndex<MetaType>::max() const
    {
	return mRangePtr->size();
    }

    template <typename MetaType>
    IndexId<0> UIndex<MetaType>::id() const
    {
	return IndexId<0>(this->ptrId());
    }

    template <typename MetaType>
    const MetaType& UIndex<MetaType>::operator*() const
    {
	return mMetaPtr[IB::mPos];
    }
    
    template <typename MetaType>
    String UIndex<MetaType>::stringMeta() const
    {
	return toString(this->meta());
    }
    
    template <typename MetaType>
    const MetaType& UIndex<MetaType>::meta() const
    {
	return mMetaPtr[IB::mPos];
    }
    
    template <typename MetaType>
    UIndex<MetaType>& UIndex<MetaType>::at(const MetaType& metaPos)
    {
	(*this) = mRangePtr->getMeta(metaPos);
	return *this;
    }

    template <typename MetaType>
    size_t UIndex<MetaType>::dim() const // = 1
    {
	return 1;
    }

    template <typename MetaType>
    Sptr<URange<MetaType>> UIndex<MetaType>::range() const
    {
	return mRangePtr;
    }

    template <typename MetaType>
    template <SizeT I>
    UPos UIndex<MetaType>::stepSize(const IndexId<I>& id) const
    {
	return UPos(id == this->id() ? 1 : 0);
    }
    
    template <typename MetaType>
    template <class Xpr, class F>
    decltype(auto) UIndex<MetaType>::ifor(const Xpr& xpr, F&& f) const
    {
	return For<0,Xpr,F>(this->max(), this->id(), xpr, std::forward<F>(f));
    }
    
    /**********************
     *   URangeFactory    *
     **********************/

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
	const auto& info = typeid(URange<MetaType>);
	if(mRef != nullptr) {
	    mProd = this->fromCreated(info, {mRef->id()});
	}
	if(mProd == nullptr){
	    RangePtr key = mProd = std::shared_ptr<URange<MetaType>>
		( new URange<MetaType>( std::move(mSpace) ) );
	    if(mRef != nullptr) { key = mRef; }
	    this->addToCreated(info, { key->id() }, mProd);
	}
    }
    
    /***************
     *   URange    *
     ***************/
    
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
    const MetaType& URange<MetaType>::get(SizeT pos) const
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
    const TypeInfo& URange<MetaType>::type() const
    {
	return typeid(URange<MetaType>);
    }
    
    template <typename MetaType>
    const TypeInfo& URange<MetaType>::metaType() const
    {
	return typeid(MetaType);
    }

    /*******************
     *   Range Casts   *
     *******************/

    template <typename MetaType>
    Sptr<URange<MetaType>> RangeCast<URange<MetaType>>::func(const RangePtr& r)
    {
	CXZ_ERROR("to be implemented...");
	return nullptr;
    }
    
}

#endif
