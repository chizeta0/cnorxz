// -*- C++ -*-
#include "index_base.h"

namespace MultiArrayTools
{
    /************************
     *  IndefinitIndexBase  *
     ************************/
    
    IndefinitIndexBase::~IndefinitIndexBase()
    {
	//freeLinked();
	mLinked = nullptr;
	mMajor = nullptr;
	mSoftLinked = nullptr;
    }
    
    size_t IndefinitIndexBase::pos() const
    {
	//assert(not virt());
	return static_cast<size_t>( mPos );
    }
    
    const std::string& IndefinitIndexBase::name() const
    {
	//assert(not virt());
	return mName;
    }

    void IndefinitIndexBase::name(const std::string& str)
    {
	//assert(not virt());
	mName = str;
    }
    
    void IndefinitIndexBase::name(const Name& nm)
    {
	//assert(not virt());
	mName = nm.own();
    }    
    
    bool IndefinitIndexBase::link(IndefinitIndexBase* toLink)
    {
	//assert(not virt());
	if(toLink->rangeType() != rangeType() and toLink->name() == name()){
	    assert(0);
	    // throw !!
	}
	if(toLink->rangeType() == rangeType() and toLink->name() == name()){
	    if(mLinked == toLink){
		return true; // dont link twice the same
	    }
	    else if(mLinked == nullptr){
		mLinked = toLink;
		return true;
	    }
	    else {
		return mLinked->link(toLink);
	    }
	}
	else {
	    return false;
	}
    }

    void IndefinitIndexBase::freeLinked()
    {
	if(mLinked != nullptr){
	    //mLinked->freeLinked();
	    mLinked = nullptr;
	}
	if(mSoftLinked != nullptr){
	    //mSoftLinked->freeLinked();
	    mSoftLinked = nullptr;
	}
    }

    bool IndefinitIndexBase::linked() const
    {
	//assert(not virt());
	return mLinked != nullptr;
    }

    void IndefinitIndexBase::setPos(size_t pos)
    {
	//assert(not virt());
	mPos = pos;
	if(linked()){
	    mLinked->setPos(mPos);
	    mLinked->evalMajor();
	}
    }

    void IndefinitIndexBase::setPosRel(int relPos)
    {
	mPos += relPos;
	if(linked()){
	    mLinked->setPosRel(relPos);
	    mLinked->evalMajor(relPos);
	}
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
