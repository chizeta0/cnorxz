
#include "index_base.h"

namespace MultiArrayTools
{
    /************************
     *  IndefinitIndexBase  *
     ************************/

    IndefinitIndexBase::~IndefinitIndexBase()
    {
	freeLinked();
	mLinked = nullptr;
	mMajor = nullptr;
	mSoftLinked = nullptr;
    }
    
    size_t IndefinitIndexBase::pos() const
    {
	return mPos;
    }
    
    const std::string& IndefinitIndexBase::name() const
    {
	return mName;
    }

    void IndefinitIndexBase::name(const std::string& str)
    {
	mName = str;
    }
    
    void IndefinitIndexBase::name(const Name& nm)
    {
	mName = nm.own();
    }    
    
    bool IndefinitIndexBase::link(IndefinitIndexBase* toLink)
    {
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
	    mLinked->freeLinked();
	    mLinked = nullptr;
	}
	if(mSoftLinked != nullptr){
	    mSoftLinked->freeLinked();
	    mSoftLinked = nullptr;
	}
    }

    bool IndefinitIndexBase::linked() const
    {
	return mLinked != nullptr;
    }

    void IndefinitIndexBase::setPos(size_t pos)
    {
	mPos = pos;
	if(linked()){
	    mLinked->setPos(pos);
	    mLinked->evalMajor();
	}
    }

    size_t IndefinitIndexBase::max() const
    {
	return static_cast<size_t>( -1 );
    }

    size_t IndefinitIndexBase::outOfRange() const
    {
	int res = pos() - max() + 1;
	return res > 0 ? static_cast<size_t>(res) : 0;
    }

    bool IndefinitIndexBase::toNull() const
    {
	return true;
    }

    void IndefinitIndexBase::evalMajor()
    {
	if(not master()){
	    mMajor->eval();
	}
    }
    
    bool IndefinitIndexBase::master()
    {
	return mMajor == nullptr;
    }

    void IndefinitIndexBase::subOrd(IndefinitIndexBase* major)
    {
	mMajor = major;
    }
    
    /**************
     *  IndexBase *	     
     **************/

    template <class Index>
    IndexBase<Index>::IndexBase(RangeBase<Index> const* range) : mRange(range) {}
    
    template <class Index>
    size_t IndexBase<Index>::max() const
    {
	return mRange->size();
    }

    template <class Index>
    bool IndexBase<Index>::toNull() const
    {
	return mRange == nullptr;
    }

    template <class Index>
    void IndexBase<Index>::assignRange(RangeBase<Index> const* range)
    {
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
