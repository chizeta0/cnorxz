
#include "ranges/rheader.h"
#include "ranges/x_to_string.h"

namespace MultiArrayTools
{
    std::shared_ptr<SpinRF> mkSPIN(char* dp, size_t size)
    {
	std::make_shared<SpinRF>();
    }
    
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

    std::string SingleRange<size_t,SpaceType::SPIN>::stringMeta(size_t pos) const
    {
	return std::to_string(get(pos));
    }

    std::vector<char> SingleRange<size_t,SpaceType::SPIN>::data() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::SPIN );
	h.metaSize = 0;
	h.multiple = 0;
	std::vector<char> out;
	out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	return out;
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

