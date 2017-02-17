
#include "range_base.h"

namespace MultiArrayTools
{
    /*********************
     *  MultiRangeType   *
     *********************/

    MultiRangeType::MultiRangeType(const RangeType& type) : mType(type), mMultiType(nullptr) {}
    
    MultiRangeType::MultiRangeType(const std::vector<MultiRangeType>& multiType) :
	mType(RangeType::NIL),
	mMultiType(new std::vector<MultiRangeType>(multiType)) {}

    MultiRangeType::~MultiRangeType()
    {
	delete mMultiType;
    }

    
    MultiRangeType& MultiRangeType::operator=(const RangeType& type)
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
	return mMultiType != nullptr;
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

    template <class Index>
    RangeBase<Index>* RangeBase<Index>::base()
    {
	return nullptr;
    }
    
    /*********************
     *   SubRangeBase    *
     *********************/

    template <class Index>
    bool SubRangeBase<Index>::isSubRange() const
    {
	return true;
    }


}
