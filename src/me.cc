// -*- C++ -*-

#include "me.h"

namespace ME
{

    /*********************
     *  MultiRangeType   *
     *********************/

    MultiRangeType& MultiRangeType::operator=(RangeType& type)
    {
	setType(type);
	return *this;
    }
    
    MultiRangeType& MultiRangeType::operator=(const std::vector<MultiRangeType>& multiType)
    {
	setMultiType(multiType);
	return *this;
    }

    MultiRangeType& MultiRangeType::operator[](size_t num)
    {
	return mMultiType->at(num);
    }
    
    const MultiRangeType& MultiRangeType::operator[](size_t num) const
    {
	return mMultiType->at(num);
    }
    
    bool MultiRangeType::multi() const
    {
	return mType != nullptr;
    }

    bool MultiRangeType::operator==(const MultiRangeType& in) const
    {
	if(multi()){
	    return *mMultiType == *in.mMultiType;
	}
	else {
	    return mType == in.mType;
	}
    }
    
    bool MultiRangeType::operator!=(const MultiRangeType& in) const
    {
	if(multi()){
	    return *mMultiType != *in.mMultiType;
	}
	else {
	    return mType != in.mType;
	}
    }
    
    void MultiRangeType::setType(RangeType type)
    {
	mType = type;
	if(mMultiType != nullptr){
	    delete mMultiType;
	}
	mMultiType = nullptr;
    }
    
    void MultiRangeType::setMultiType(const std::vector<MultiRangeType>& multiType)
    {
	mMultiType = new std::vector<MultiRangeType>( multiType );
	mType = RangeType::NIL;
    }
    
    /******************
     *   RangeBase    *
     ******************/

    template <class Index>
    bool RangeBase<Index>::isSubRange() const
    {
	return false;
    }

    /*********************
     *   SubRangeBase    *
     *********************/

    template <class Index>
    bool SubRangeBase<Index>::isSubRange() const
    {
	return true;
    }

    /********************
     *   SingleRange    *
     ********************/

    template <typename U, RangeType TYPE>
    const U& SingleRange<U,TYPE>::get(size_t pos) const
    {
	return mSpace[pos];
    }

    template <typename U, RangeType TYPE>
    size_t SingleRange<U,TYPE>::get(const U& metaPos) const
    {
	size_t cnt = 0;
	for(auto& x: mSpace){
	    if(x == metaPos){
		return cnt;
	    }
	    ++cnt;
	}
	return cnt;
    }

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
	mLinked = nullptr;
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

    /********************
     *  SingleIndexBase *	     
     ********************/
    
    template <typename U, IndexType TYPE>
    const U& SingleIndexBase<U,TYPE>::getMetaPos() const
    {
	return dynamic_cast<SingleRange*>( mRange )->get(mPos);
    }

    template <typename U, IndexType TYPE>
    size_t SingleIndexBase<TYPE>::evaluate(const Index& in)
    {
	return in.mPos;
    }
    

    /*******************
     *  MultiIndexBase *	     
     *******************/

    namespace
    {
	template <size_t N, class MultiIndex>
	size_t evaluate_x(const MultiIndex& index)
	{
	    const auto& subIndex = index.getIndex<N>();
	    return evaluate_x<N-1>(index) * subIndex.size() + subIndex.pos();
	}

	template <class MultiIndex>
	size_t evaluate_x<0>(const MultiIndex& index)
	{
	    const auto& subIndex = index.getIndex<0>();
	    return subIndex.pos();
	}
    }
    
    template <class... Indices>
    size_t MultiIndex<Indices...>::evaluate(const MultiIndex<Indices...>& in) const
    {
	return evaluate_x<sizeof...(Indices)-1>(in);
    }

    template <class... Indices>
    bool MultiIndex<Indices...>::link(IndefinitIndexBase* toLink)
    {
	if(toLink->name() == name() and toLink->rangeType() == rangeType()){
	    bool isAlready = false;
	    if(mLinked != nullptr){
		for(auto& x: *mLinked){
		    if(x == toLink){
			isAlready = true;
			break;
		    }
		}
	    }
	    else {
		mLinked = new std::vector<IndefinitIndexBase*>();
	    }
	    if(not isAlready){
		mLinked->push_back(toLink);
	    }
	    return true;
	}
	else {
	    return /*try each element in mIPack*/;
	}
    }
    
    /*******************
     *  MultiArray     *	     
     *******************/

    template <typename T, class Range>
    T& MultiArray<T,Is...>::operator()(const typename Range::indexType& i)
    {
	return i.pos();
    }

    template <typename T, class Range>
    const T& MultiArray<T,Is...>::operator()(const typename Range::indexType& i) const
    {
	return i.pos();
    }

    
    /***************************
     *   NamedMultiArray       *
     ***************************/

    IndefinitIndexBase& getIndex(const std::string& name)
    {
	return mIndexNameMap.at(name);
    }

    const IndefinitIndexBase& getIndex(const std::string& name) const
    {
	return mIndexNameMap.at(name);
    }
    
} // end namespace ME

