
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

    template <typename MetaType>
    UIndex<MetaType>::UIndex(const RangePtr& range, SizeT pos) :
	IndexInterface<UIndex<MetaType>,MetaType>(pos),
	mRangePtr(rangeCast<MetaType>(range)),
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
    const MetaType& UIndex<MetaType>::operator*() const
    {
	return mMetaPtr[IB::mPos];
    }
    
    template <typename MetaType>
    const MetaType* UIndex<MetaType>::operator->() const
    {
	return mMetaPtr + IB::mPos;
    }
	
    template <typename MetaType>
    Int UIndex<MetaType>::pp(PtrId idxPtrNum)
    {
	++(*this);
	return 1;
    }

    template <typename MetaType>
    Int UIndex<MetaType>::mm(PtrId idxPtrNum)
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
    SizeT UIndex<MetaType>::getStepSize(PtrId iptr) const
    {
	return iptr == this->ptrId() ? 1 : 0;
    }
    
    template <typename MetaType>
    Int UIndex<MetaType>::getOffset(PtrId iptr) const
    {
	return 0;
    }
    /*
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
    */
    
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
	auto info = typeid(URange<MetaType>);
	if(mRef != nullptr) {
	    mProd = this->fromCreated[info.hash_code()][mRef->id()];
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
	RangeInterface<URange<MetaType>,MetaType>(),
	mSpace(space)
    {
	std::sort(mSpace.begin(), mSpace.end(), std::less<MetaType>());
	auto itdupl = std::adjacent_find(mSpace.begin(), mSpace.end());
	CXZ_ASSERT(itdupl == mSpace.end(), "found duplicate: " << *itdupl);
    }

    template <typename MetaType>
    URange<MetaType>::URange(Vector<MetaType>&& space) :
	RangeInterface<URange<MetaType>,MetaType>(),
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
    typename URange<MetaType>::IndexType URange<MetaType>::begin() const
    {
	UIndex<MetaType> i( std::dynamic_pointer_cast<URange<MetaType> >
			       ( RangePtr( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    template <typename MetaType>
    typename URange<MetaType>::IndexType URange<MetaType>::end() const
    {
	UIndex<MetaType> i( std::dynamic_pointer_cast<URange<MetaType> >
			       ( RangePtr( RB::mThis ) ) );
	i = this->size();
	return i;
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
