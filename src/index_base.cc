// -*- C++ -*-
#include "index_base.h"

namespace MultiArrayTools
{
    /************************
     *  IndefinitIndexBase  *
     ************************/
    
    IndefinitIndexBase::~IndefinitIndexBase()
    {
	mMajor = nullptr;
    }
    
    size_t IndefinitIndexBase::pos() const
    {
	//assert(not virt());
	return static_cast<size_t>( mPos );
    }
    
    void IndefinitIndexBase::setPos(size_t pos)
    {
	//CHECK;
	//assert(not virt());
	mPos = pos;
    }

    void IndefinitIndexBase::setPosRel(int relPos)
    {
	mPos += relPos;
    }
    // MAJOR INDEX UPDATE !!!!!
    void IndefinitIndexBase::toFirst()
    {
	mPos = 0;
    }
    
    void IndefinitIndexBase::toLast()
    {
	mPos = max() - 1;
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

    bool atEdge() const
    {
	return mPos == max();
    }
    
    bool IndefinitIndexBase::toNull() const
    {
	//assert(not virt());
	return true;
    }

    void IndefinitIndexBase::evalMajor()
    {
	//assert(not virt());
	if(not master()){
	    //int start = mMajor->pos();
	    mMajor->eval();
	    //VCHECK(static_cast<int>( mMajor->pos() ) - start);
	}
    }
    
    void IndefinitIndexBase::evalMajor(int num)
    {
	//assert(not virt());
	//CHECK;
	if(not master()){
	    //int start = mMajor->pos();
	    mMajor->setPosRel( num * mMajorStep);
	    //VCHECK(static_cast<int>( mMajor->pos() ) - start);
	}
    }
    
    bool IndefinitIndexBase::master()
    {
	//assert(not virt());
	return mMajor == nullptr;
    }

    void IndefinitIndexBase::subOrd(IndefinitIndexBase* major)
    {
	//assert(not virt());
	mMajor = major;
	mMajorStep = mMajor->giveSubStepSize(this);
    }

    size_t IndefinitIndexBase::majorStep() const
    {
	return mMajorStep;
    }
    
    /**************
     *  IndexBase *	     
     **************/

    template <class Index>
    IndexBase<Index>::IndexBase(RangeBase<Index> const* range) : mRange(range) {}
    
    template <class Index>
    size_t IndexBase<Index>::max() const
    {
	//assert(not virt());
	return mRange->size();
    }

    template <class Index>
    bool IndexBase<Index>::toNull() const
    {
	//assert(not virt());
	return mRange == nullptr;
    }

    template <class Index>
    void IndexBase<Index>::assignRange(RangeBase<Index> const* range)
    {
	//assert(not virt());
	if(toNull()){
	    mRange = range;
	}
    }
    
    template <class Index>
    void IndexBase<Index>::eval()
    {
	setPos( evaluate(*dynamic_cast<Index const*>( this )) );
    }
}
