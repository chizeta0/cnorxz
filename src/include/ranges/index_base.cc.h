
#ifndef __cxz_index_base_cc_h__
#define __cxz_index_base_cc_h__

#include "index_base.h"

namespace CNORXZ
{
    /**********************
     *   IndexInterface   *
     **********************/

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>::IndexInterface()
    {
	mPtrNum = reinterpret_cast<PtrId>(this);
    }

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>::IndexInterface(const IndexInterface& in) :
	mRangePtr(in.mRangePtr), mPos(in.mPos), mMax(in.mMax)
    {
	mPtrNum = reinterpret_cast<PtrId>(this);
    }

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>::IndexInterface(IndexInterface&& in) :
	mRangePtr(in.mRangePtr), mPos(in.mPos), mMax(in.mMax)
    {
	mPtrNum = reinterpret_cast<PtrId>(this);
    }

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>& IndexInterface<I,MetaType>::operator=(const IndexInterface& in)
    {
	mRangePtr = in.mRangePtr;
	mPos = in.mPos;
	mMax = in.mMax;
	return *this;
    }

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>& IndexInterface<I,MetaType>::operator=(IndexInterface&& in)
    {
	mRangePtr = in.mRangePtr;
	mPos = in.mPos;
	mMax = in.mMax;
	return *this;
    }
    
    template <class I, typename MetaType>
    IndexInterface<I,MetaType>::IndexInterface(const RangePtr& range, SizeT pos) :
	mRangePtr(range), mPos(pos), mMax(mRangePtr->size())
    {
	mPtrNum = reinterpret_cast<PtrId>(this);
    }

    template <class I, typename MetaType>
    bool IndexInterface<I,MetaType>::operator==(const IndexInterface& in) const
    {
	return in.mPos == mPos and in.mRangePtr.get() == mRangePtr.get();
    }

    template <class I, typename MetaType>
    bool IndexInterface<I,MetaType>::operator!=(const IndexInterface& in) const
    {
	return in.mPos != mPos or in.mRangePtr.get() != mRangePtr.get();
    }

    template <class I, typename MetaType>
    bool IndexInterface<I,MetaType>::operator<(const IndexInterface& in) const
    {
	return mPos < in.mPos;
    }

    template <class I, typename MetaType>
    bool IndexInterface<I,MetaType>::operator>(const IndexInterface& in) const
    {
	return mPos > in.mPos;
    }

    template <class I, typename MetaType>
    bool IndexInterface<I,MetaType>::operator<=(const IndexInterface& in) const
    {
	return mPos <= in.mPos;
    }

    template <class I, typename MetaType>
    bool IndexInterface<I,MetaType>::operator>=(const IndexInterface& in) const
    {
	return mPos >= in.mPos;
    }

    template <class I, typename MetaType>
    SizeT IndexInterface<I,MetaType>::pos() const
    {
	return mPos;
    }

    template <class I, typename MetaType>
    SizeT IndexInterface<I,MetaType>::max() const
    {
	return mMax;
    }

    template <class I, typename MetaType>
    PtrId IndexInterface<I,MetaType>::ptrId() const
    {
	// if this assert never applies, remove mPtrId (-> Defaults) !!!
	assert(mPtrId == reinterpret_cast<PtrId>(this));
	return mPtrId;
    }
}

#endif
