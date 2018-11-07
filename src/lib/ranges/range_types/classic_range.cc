
#include "ranges/rheader.h"
#include "ranges/x_to_string.h"

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

    SpaceType SingleRange<size_t,SpaceType::NONE>::spaceType() const
    {
	return SpaceType::NONE;
    }
    
    std::string SingleRange<size_t,SpaceType::NONE>::stringMeta(size_t pos) const
    {
	return std::to_string(get(pos));
    }

    std::vector<char> SingleRange<size_t,SpaceType::NONE>::data() const
    {
	DataHeader h = dataHeader();
	std::vector<char> out;
	out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	const char* scp = reinterpret_cast<const char*>(&mSize);
	out.insert(out.end(), scp, scp + h.metaSize);
	return out;
    }

    DataHeader SingleRange<size_t,SpaceType::NONE>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::NONE );
	h.metaSize = sizeof(size_t);
	h.multiple = 0;
        return h;
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

