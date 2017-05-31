// -*- C++ -*-
#include "index_base.h"

namespace MultiArrayTools
{
    /************************
     *  IndefinitIndexBase  *
     ************************/
    
    size_t IndefinitIndexBase::pos() const
    {
	//assert(not virt());
	return static_cast<size_t>( mPos );
    }

    bool IndefinitIndexBase::operator==(const IndefinitIndexBase& in) const
    {
	return rangeType() == in.rangeType() and pos() == in.pos();
    }
    
    bool IndefinitIndexBase::operator!=(const IndefinitIndexBase& in) const
    {
	return rangeType() != in.rangeType() or pos() != in.pos();
    }
    
    IndefinitIndexBase& IndefinitIndexBase::setPos(size_t pos, IndefinitIndexBase* ctrlPtr)
    {
	mPos = pos;
	for(auto mm: mMajor){
	    if(mm.first == ctrlPtr){
		continue;
	    }
	    mm.first->setPos(mm.first->pos() % mm.second + mm.second * pos, this);
	}
	return *this;
    }

    IndefinitIndexBase& IndefinitIndexBase::setPosRel(int relPos, IndefinitIndexBase* ctrlPtr)
    {
	mPos += relPos;
	for(auto mm: mMajor){
	    if(mm.first == ctrlPtr){
		continue;
	    }
	    mm.first->setPosRel(mm.second * relPos, this);
	}
	return *this;
    }

    IndefinitIndexBase& IndefinitIndexBase::toFirst(IndefinitIndexBase* ctrlPtr)
    {
	mPos = 0;
	for(auto mm: mMajor){
	    if(mm.first == ctrlPtr){
		continue;
	    }
	    mm.first->setPos(mm.first->pos() % mm.second, this);
	}
	return *this;
    }
    
    IndefinitIndexBase& IndefinitIndexBase::toLast(IndefinitIndexBase* ctrlPtr)
    {
	mPos = max() - 1;
	for(auto mm: mMajor){
	    if(mm.first == ctrlPtr){
		continue;
	    }
	    mm.first->setPos(mm.first->pos() % mm.second + mm.second * mPos, this);
	}
	return *this;
    }
    
    int IndefinitIndexBase::outOfRange() const
    {
	if(mPos < 0){
	    return mPos;
	}
	else if(mPos >= static_cast<int>( max() ) ){
	    return mPos - max() + 1;
	}
	else {
	    return 0;
	}
    }

    bool IndefinitIndexBase::atEdge() const
    {
	return static_cast<size_t>( mPos ) == max();
    }
    
    bool IndefinitIndexBase::master() const
    {
	return mMajor.size() == 0;
    }

    IndefinitIndexBase& IndefinitIndexBase::subOrd(IndefinitIndexBase* major)
    {
	mMajor[major] = major->giveSubStepSize(this);
	return *this;
    }
    
    /**************
     *  IndexBase *	     
     **************/

    template <class Index>
    IndexBase<Index>::IndexBase(std::shared_ptr<const RangeBase<Index> >& rangePtr) : mRangePtr(rangePtr) {}
    
    template <class Index>
    size_t IndexBase<Index>::max() const
    {
	//assert(not virt());
	return mRange->size();
    }
}
