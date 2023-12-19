
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

    DIndex::DIndex(const RangePtr& r, SizeT lexpos) :
	IndexInterface<DIndex,DType>(0)
    {
	*this = r->begin();
	*this = lexpos;
    }

    DIndex& DIndex::operator=(SizeT lexpos)
    {
	*mI = lexpos;
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

    SizeT DIndex::operator-(const DIndex& i) const
    {
	return (*mI) - (*i.mI);
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

    SizeT DIndex::lex() const
    {
	return mI->lex();
    }
    
    UPos DIndex::pmax() const
    {
	return mI->pmax();
    }
    
    UPos DIndex::lmax() const
    {
	return mI->lmax();
    }
    
    IndexId<0> DIndex::id() const
    {
	return mI->id();
    }

    DType DIndex::operator*() const
    {
	return *(*mI);
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
    /*
    Vector<XIndexPtr> DIndex::pack() const
    {
	return mI->pack();
    }

    Vector<SizeT> DIndex::blockSizes() const
    {
	return mI->blockSizes();
    }
    
    DIndex& DIndex::setBlockSizes(const Vector<SizeT>& bs)
    {
	mI->setBlockSizes(bs);
	return *this;
    }
    */
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

    RangePtr DIndex::prange(const DIndex& end) const
    {
	return mI->prange( end.xptr() );
    }

    Vector<SizeT> DIndex::deepFormat() const
    {
	return mI->deepFormat();
    }

    Vector<SizeT> DIndex::deepMax() const
    {
	return mI->deepMax();
    }

    DIndex DIndex::reformat(const Vector<SizeT>& f, const Vector<SizeT>& s) const
    {
	return DIndex(mI->reformat(f,s));
    }

    bool DIndex::formatIsTrivial() const
    {
	return true;
    }

    const XIndexPtr& DIndex::xptr() const
    {
	return mI;
    }
}
