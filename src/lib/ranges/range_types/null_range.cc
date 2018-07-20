
#include "ranges/rheader.h"

namespace MultiArrayTools
{
    /********************
     *   SingleRange    *
     ********************/

    std::shared_ptr<NullRange> nullr()
    {
	return SingleRangeFactory<size_t,SpaceType::NUL>::mRInstance;
    }

    std::shared_ptr<NullIndex> nulli()
    {
	return std::make_shared<NullIndex>(nullr());
    }
    
    std::shared_ptr<SingleRange<size_t,SpaceType::NUL>>
    SingleRangeFactory<size_t,SpaceType::NUL>::mRInstance = nullptr;

    SingleRangeFactory<size_t,SpaceType::NUL>::SingleRangeFactory()
    {
	// Singleton
	if( not mRInstance){
	    if(not mProd){
		mProd = std::shared_ptr<oType>( new SingleRange<size_t,SpaceType::NUL>() );
		setSelf();
	    }
	} else {
	    mProd = mRInstance;
	}
    }

    std::shared_ptr<RangeBase> SingleRangeFactory<size_t,SpaceType::NUL>::create()
    {
	return mProd;
    }
    
    /********************
     *   SingleRange    *
     ********************/
    
    size_t SingleRange<size_t,SpaceType::NUL>::get(size_t pos) const
    {
	return 0;
    }
    
    size_t SingleRange<size_t,SpaceType::NUL>::getMeta(size_t metapos) const
    {
	return 0;
    }
    
    size_t SingleRange<size_t,SpaceType::NUL>::size() const
    {
	return 1;
    }
    
    size_t SingleRange<size_t,SpaceType::NUL>::dim() const
    {
	return 1;
    }
    
    typename SingleRange<size_t,SpaceType::NUL>::IndexType SingleRange<size_t,SpaceType::NUL>::begin() const
    {
	SingleIndex<size_t,SpaceType::NUL> i( std::dynamic_pointer_cast<SingleRange<size_t,SpaceType::NUL> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    typename SingleRange<size_t,SpaceType::NUL>::IndexType SingleRange<size_t,SpaceType::NUL>::end() const
    {
	SingleIndex<size_t,SpaceType::NUL> i( std::dynamic_pointer_cast<SingleRange<size_t,SpaceType::NUL> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }

    // put this in the interface class !!!
    /*
    std::shared_ptr<VIWB> SingleRange<size_t,SpaceType::NUL>::index() const
    {
	typedef IndexWrapper<IndexType> IW;
	return std::make_shared<IW>
            ( std::make_shared<IndexType>
	      ( std::dynamic_pointer_cast<SingleRange<size_t,SpaceType::NUL> >
		( std::shared_ptr<RangeBase>( RB::mThis ) ) ) );
    }
    */
}

