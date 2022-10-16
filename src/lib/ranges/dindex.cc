
#include "ranges/ranges.h"

namespace CNORXZ
{
    DIndex::DIndex(const DIndex& i) :
	IndexInterface<DIndex,DType>(i),
	mI(i.mI->copy())
    {}
    
    DIndex::DIndex(DIndex&& i) :
	IndexInterface<DIndex,DType>(i),
	mI(i.mI)
    {
	i.mI = nullptr;
    }
    
    DIndex& DIndex::operator=(const DIndex& i)
    {
	IndexInterface<DIndex,DType>::operator=(i);
	mI = i.mI->copy();
	IB::mPos = mI->pos();
	return *this;
    }
    
    DIndex& DIndex::operator=(DIndex&& i)
    {
	IndexInterface<DIndex,DType>::operator=(i);
	mI = i.mI;
	IB::mPos = mI->pos();
	i.mI = nullptr;
	return *this;
    }
    
    DIndex::DIndex(const XIndexPtr& i) :
	IndexInterface<DIndex,DType>(i->pos()),
	mI(i)
    {}
    
    DIndex& DIndex::operator=(SizeT pos)
    {
	*mI = pos;
	IB::mPos = mI->pos();
	return *this;
    }
    
    DIndex& DIndex::operator++()
    {
	++(*mI);
	IB::mPos = mI->pos();
	return *this;
    }
    
    DIndex& DIndex::operator--()
    {
	--(*mI);
	IB::mPos = mI->pos();
	return *this;
    }
    
    DIndex DIndex::operator+(Int n) const
    {
	return (*mI) + n;
    }
    
    DIndex DIndex::operator-(Int n) const
    {
	return (*mI) - n;
    }
    
    DIndex& DIndex::operator+=(Int n)
    {
	(*mI) += n;
	IB::mPos = mI->pos();
	return *this;
    }
    
    DIndex& DIndex::operator-=(Int n)
    {

	IB::mPos = mI->pos();
	return *this;
    }

    DType DIndex::operator*() const
    {
	return *(*mI);
    }
    
    DType DIndex::operator->() const
    {
	return *(*mI);
    }

    Int DIndex::pp(PtrId idxPtrNum)
    {
	const Int out = mI->pp(idxPtrNum);
	IB::mPos = mI->pos();
	return out;
    }
    
    Int DIndex::mm(PtrId idxPtrNum)
    {
	const Int out = mI->mm(idxPtrNum);
	IB::mPos = mI->pos();
	return out;
    }

    SizeT DIndex::dim() const
    {
	return mI->dim();
    }
    
    RangePtr DIndex::range() const
    {
	return mI->range();
    }
    
    UPos DIndex::stepSize(const IndexId<0>& id) const
    {
	return mI->stepSize(id);
    }

    String DIndex::stringMeta() const
    {
	return mI->stringMeta();
    }
    
    DType DIndex::meta() const
    {
	return mI->meta();
    }
    
    DIndex& DIndex::at(const DType& meta)
    {
	mI->at(meta);
	IB::mPos = mI->pos();
	return *this;
    }

    DXpr DIndex::ifor(const UPos& step, const DXpr& xpr) const
    {
	return DXpr(mI->ifor(step, xpr));
    }
    
}
