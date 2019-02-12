
#include "ranges/rheader.h"
#include "ranges/x_to_string.h"

namespace MultiArrayTools
{
    /********************
     *   GenSingleRange    *
     ********************/
    
    GenSingleRangeFactory<size_t,SpaceType::NONE,-1>::GenSingleRangeFactory(size_t size)
    {
	// Quasi Singleton
	if(not mProd){
	    mProd = std::shared_ptr<oType>( new GenSingleRange<size_t,SpaceType::NONE,-1>(size) );
	    setSelf();
	}
    }

    std::shared_ptr<RangeBase> GenSingleRangeFactory<size_t,SpaceType::NONE,-1>::create()
    {
	return mProd;
    }
    
    /********************
     *   GenSingleRange    *
     ********************/

    GenSingleRange<size_t,SpaceType::NONE,-1>::GenSingleRange(size_t size) : mSize(size) {}
    
    size_t GenSingleRange<size_t,SpaceType::NONE,-1>::get(size_t pos) const
    {
	return pos;
    }
    
    size_t GenSingleRange<size_t,SpaceType::NONE,-1>::getMeta(size_t metaPos) const
    {
	return metaPos;
    }
    
    size_t GenSingleRange<size_t,SpaceType::NONE,-1>::size() const
    {
	return mSize;
    }
    
    size_t GenSingleRange<size_t,SpaceType::NONE,-1>::dim() const
    {
	return 1;
    }

    SpaceType GenSingleRange<size_t,SpaceType::NONE,-1>::spaceType() const
    {
	return SpaceType::NONE;
    }
    
    std::string GenSingleRange<size_t,SpaceType::NONE,-1>::stringMeta(size_t pos) const
    {
	return std::to_string(get(pos));
    }

    std::vector<char> GenSingleRange<size_t,SpaceType::NONE,-1>::data() const
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

    DataHeader GenSingleRange<size_t,SpaceType::NONE,-1>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::NONE );
	h.metaSize = sizeof(size_t);
	h.multiple = 0;
        return h;
    }

    
    typename GenSingleRange<size_t,SpaceType::NONE,-1>::IndexType GenSingleRange<size_t,SpaceType::NONE,-1>::begin() const
    {
	GenSingleIndex<size_t,SpaceType::NONE,-1> i( std::dynamic_pointer_cast<GenSingleRange<size_t,SpaceType::NONE,-1> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    typename GenSingleRange<size_t,SpaceType::NONE,-1>::IndexType GenSingleRange<size_t,SpaceType::NONE,-1>::end() const
    {
	GenSingleIndex<size_t,SpaceType::NONE,-1> i( std::dynamic_pointer_cast<GenSingleRange<size_t,SpaceType::NONE,-1> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }
}

