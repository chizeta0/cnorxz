
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
    
    SizeT DIndex::getStepSize(PtrId iptr) const
    {
	return mI->getStepSize(iptr);
    }

    Int DIndex::getOffset(PtrId iptr) const
    {
	return mI->getOffset(iptr);
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

    //DExpr DIndex::ifor(SizeT step, DExpr ex) const;
    //DExpr DIndex::iforh(SizeT step, DExpr ex) const;
    
}
