
#include "index_base.h"

namespace MultiArrayTools
{
    /************************
     *  IndefinitIndexBase  *
     ************************/

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
	if(linked()){
	    mLinked->freeLinked();
	    mLinked = nullptr;
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
	}
    }

    size_t IndefinitIndexBase::max() const
    {
	return static_cast<size_t>( -1 );
    }

    size_t IndefinitIndexBase::outOfRange() const
    {
	int res = pos() - max();
	return res > 0 ? static_cast<size_t>(res) : 0;
    }
    
    /**************
     *  IndexBase *	     
     **************/

    template <class Index>
    size_t IndexBase<Index>::max() const
    {
	return mRange->size();
    }
}
