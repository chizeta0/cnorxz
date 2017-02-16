
#include "index_base.h"

namespace MultiArrayTools
{
    /************************
     *  IndefinitIndexBase  *
     ************************/

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
    Index& IndexBase<Index>::operator=(const Index& in)
    {
	setPos( evaluate(in) );
    }

    template <class Index>
    Index& IndexBase<Index>::operator=(size_t pos)
    {
	setPos( pos );
	return *this;
    }

    template <class Index>
    Index& IndexBase<Index>::operator++()
    {
	setPos( ++mPos );
	return *this;
    }

    template <class Index>
    Index& IndexBase<Index>::operator--()
    {
	setPos( --mPos );
	return *this;
    }

    template <class Index>
    Index& IndexBase<Index>::operator+=(int n)
    {
	setPos( mPos += n );
	return *this;
    }

    template <class Index>
    Index& IndexBase<Index>::operator-=(int n)
    {
	setPos( mPos -= n );
	return *this;
    }

    template <class Index>
    bool IndexBase<Index>::operator==(const Index& i)
    {
	return mRange == i.mRange and mPos == i.mPos;
    }

    template <class Index>
    bool IndexBase<Index>::operator!=(const Index& i)
    {
	return mRange != i.mRange or mPos != i.mPos;
    }

    template <class Index>
    size_t IndexBase<Index>::max() const
    {
	return mRange->size();
    }


}
