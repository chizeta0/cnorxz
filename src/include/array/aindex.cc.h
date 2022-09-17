
#ifndef __cxz_aindex_cc_h__
#define __cxz_aindex_cc_h__

#include "aindex.h"
#include "statics/static_for.h"

namespace CNORXZ
{

    template <typename T>
    AIndex<T>::AIndex(const T* data, const RangePtr& range, SizeT pos):
	IndexInterface<AIndex<T>,DType>(pos),
	mRangePtr(rangeCast<YRange>(range)), mIs(mRangePtr->dim()),
	mBlockSizes(mRangePtr->dim()), mExternalControl(false),
	mCData(data)
    {
	assert(0);
	// init ...!!!
    }
	
    template <typename T>
    AIndex<T>& AIndex<T>::operator=(SizeT pos)
    {
	IB::mPos = pos;
	assert(0);
	// sub inds... (LAZY!!!) !!!
	return *this;
    }

    template <typename T>
    AIndex<T>& AIndex<T>::operator++()
    {
	if(mExternalControl) this->sync();
	assert(0);
	// increment sub inds (LAZY!!!) !!!
	++IB::mPos;
	return *this;
    }

    template <typename T>
    AIndex<T>& AIndex<T>::operator--()
    {
	if(mExternalControl) this->sync();
	assert(0);
	// decrement sub inds (LAZY!!!) !!!
	--IB::mPos;
	return *this;
    }

    template <typename T>
    AIndex<T> AIndex<T>::operator+(Int n) const
    {
	assert(0);
	// sub inds !!!
	return AIndex<T>(mCData, mRangePtr, IB::mPos + n);
    }
    
    template <typename T>
    AIndex<T> AIndex<T>::operator-(Int n) const
    {
	assert(0);
	// sub inds !!!
	return AIndex<T>(mCData, mRangePtr, IB::mPos - n);
    }
    
    template <typename T>
    AIndex<T>& AIndex<T>::operator+=(Int n)
    {
	assert(0);
	// sub inds !!!
	IB::mPos += n;
	return *this;
    }
    
    template <typename T>
    AIndex<T>& AIndex<T>::operator-=(Int n)
    {
	assert(0);
	// sub inds !!!
	IB::mPos -= n;
	return *this;
    }

    template <typename T>
    const T& AIndex<T>::operator*() const
    {
	assert(0);
	// sub inds !!!
	return mCData[IB::mPos];
    }
    
    template <typename T>
    const T* AIndex<T>::operator->() const
    {
	assert(0);
	// sub inds !!!
	return mCData+IB::mPos;
    }
	
    template <typename T>
    int AIndex<T>::pp(PtrId idxPtrNum)
    {
	assert(0);
	// sub inds !!!
	return 0;
    }

    template <typename T>
    int AIndex<T>::mm(PtrId idxPtrNum)
    {
	assert(0);
	// sub inds !!!
	return 0;
    }

    template <typename T>
    SizeT AIndex<T>::dim() const
    {
	return mIs.size();
    }

    template <typename T>
    RangePtr AIndex<T>::range() const
    {
	return mRangePtr;
    }

    template <typename T>
    SizeT AIndex<T>::getStepSize(SizeT n) const
    {
	assert(0);
	// sub inds !!!
	return 0;
    }

    template <typename T>
    String AIndex<T>::stringMeta() const
    {
	String out = "[";
	auto it = mIs.begin();
	for(; it != mIs.end()-1; ++it){
	    out += (*it)->stringMeta() + ",";
	}
	out += (*it)->stringMeta() + "]";
	return out;
    }

    template <typename T>
    DType AIndex<T>::meta() const
    {
	//this->sync();
	Vector<DType> v(mIs.size());
	std::transform(mIs.begin(), mIs.end(), v.begin(), [](auto& x) { return x->meta(); });
	return DType(v);
    }

    template <typename T>
    AIndex<T>& AIndex<T>::at(const DType& meta)
    {
	auto& v = std::any_cast<const Vector<DType>&>(meta.get());
	assert(v.size() == mIs.size());
	for(SizeT i = 0; i != mIs.size(); ++i){
	    mIs[i]->at(v[i]);
	}
	return *this;
    }
    /*
    template <typename T>
    DExpr AIndex<T>::ifor(SizeT step, DExpr ex) const
    {
	return mI->ifor(step, ex);
    }

    template <typename T>
    DExpr AIndex<T>::iforh(SizeT step, DExpr ex) const
    {
	return mI->iforh(step, ex);
    }
    */

    template <typename T>
    BIndex<T>::BIndex(T* data, const RangePtr& range, SizeT pos) :
	AIndex<T>(data, range, pos), mData(data) {}

    template <typename T>
    BIndex<T>::BIndex(T* data, const AIndex<T>& ai, SizeT pos) :
	AIndex<T>(data, ai.range(), pos), mData(data) {}

    template <typename T>
    BIndex<T> BIndex<T>::operator+(Int n) const
    {
	return BIndex<T>(mData, IB::mRangePtr, IB::mPos + n);
    }
    
    template <typename T>
    BIndex<T> BIndex<T>::operator-(Int n) const
    {
	return BIndex<T>(mData, IB::mRangePtr, IB::mPos - n);
    }

    template <typename T>
    T& BIndex<T>::operator*()
    {
	return mData[AI::mPos];
    }

    template <typename T>
    T* BIndex<T>::operator->()
    {
	return mData+AI::mPos;
    }

}

#endif
