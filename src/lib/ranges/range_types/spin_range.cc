
#include "ranges/rheader.h"

namespace MultiArrayTools
{
    /********************
     *   SingleRange    *
     ********************/
    
    SingleRangeFactory<size_t,SpaceType::SPIN>::SingleRangeFactory()
    {
	// Quasi Singleton
	if(not mProd){
	    mProd = std::shared_ptr<oType>( new SingleRange<size_t,SpaceType::SPIN>() );
	    setSelf();
	}
    }

    std::shared_ptr<RangeBase> SingleRangeFactory<size_t,SpaceType::SPIN>::create()
    {
	return mProd;
    }
    
    /********************
     *   SingleRange    *
     ********************/
    
    size_t SingleRange<size_t,SpaceType::SPIN>::get(size_t pos) const
    {
	return pos;
    }
    
    size_t SingleRange<size_t,SpaceType::SPIN>::getMeta(size_t metaPos) const
    {
	return metaPos;
    }
    
    size_t SingleRange<size_t,SpaceType::SPIN>::size() const
    {
	return mSpinNum;
    }
    
    size_t SingleRange<size_t,SpaceType::SPIN>::dim() const
    {
	return 1;
    }
    
    typename SingleRange<size_t,SpaceType::SPIN>::IndexType SingleRange<size_t,SpaceType::SPIN>::begin() const
    {
	SingleIndex<size_t,SpaceType::SPIN> i( std::dynamic_pointer_cast<SingleRange<size_t,SpaceType::SPIN> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    typename SingleRange<size_t,SpaceType::SPIN>::IndexType SingleRange<size_t,SpaceType::SPIN>::end() const
    {
	SingleIndex<size_t,SpaceType::SPIN> i( std::dynamic_pointer_cast<SingleRange<size_t,SpaceType::SPIN> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }

    // put this in the interface class !!!
    /*
    std::shared_ptr<VIWB> SingleRange<size_t,SpaceType::SPIN>::index() const
    {
	typedef IndexWrapper<IndexType> IW;
	return std::make_shared<IW>
            ( std::make_shared<IndexType>
	      ( std::dynamic_pointer_cast<SingleRange<size_t,SpaceType::SPIN> >
		( std::shared_ptr<RangeBase>( RB::mThis ) ) ) );
    }
    */
}

