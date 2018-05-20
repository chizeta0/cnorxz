
#include "ranges/rheader.h"

namespace MultiArrayTools
{
    /********************
     *   SingleRange    *
     ********************/
    
    SingleRangeFactory<size_t,SpaceType::NONE>::SingleRangeFactory(size_t size)
    {
	// Quasi Singleton
	if(not mProd){
	    mProd = std::shared_ptr<oType>( new SingleRange<size_t,SpaceType::NONE>(size) );
	    setSelf();
	}
    }

    std::shared_ptr<RangeBase> SingleRangeFactory<size_t,SpaceType::NONE>::create()
    {
	return mProd;
    }
    
    /********************
     *   SingleRange    *
     ********************/

    SingleRange<size_t,SpaceType::NONE>::SingleRange(size_t size) : mSize(size) {}
    
    size_t SingleRange<size_t,SpaceType::NONE>::get(size_t pos) const
    {
	return pos;
    }
    
    size_t SingleRange<size_t,SpaceType::NONE>::getMeta(size_t metaPos) const
    {
	return metaPos;
    }
    
    size_t SingleRange<size_t,SpaceType::NONE>::size() const
    {
	return mSize;
    }
    
    size_t SingleRange<size_t,SpaceType::NONE>::dim() const
    {
	return 1;
    }
    
    typename SingleRange<size_t,SpaceType::NONE>::IndexType SingleRange<size_t,SpaceType::NONE>::begin() const
    {
	SingleIndex<size_t,SpaceType::NONE> i( std::dynamic_pointer_cast<SingleRange<size_t,SpaceType::NONE> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    typename SingleRange<size_t,SpaceType::NONE>::IndexType SingleRange<size_t,SpaceType::NONE>::end() const
    {
	SingleIndex<size_t,SpaceType::NONE> i( std::dynamic_pointer_cast<SingleRange<size_t,SpaceType::NONE> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }
}

