
#include "ranges/rheader.h"
#include "ranges/x_to_string.h"

namespace MultiArrayTools
{
    /********************
     *   SingleRange    *
     ********************/
    
    SingleRangeFactory<int,SpaceType::PSPACE>::SingleRangeFactory(size_t size)
    {
	// Quasi Singleton
	if(not mProd){
	    mProd = std::shared_ptr<oType>( new SingleRange<int,SpaceType::PSPACE>(size) );
	    setSelf();
	}
    }

    std::shared_ptr<RangeBase> SingleRangeFactory<int,SpaceType::PSPACE>::create()
    {
	return mProd;
    }
    
    /********************
     *   SingleRange    *
     ********************/

    SingleRange<int,SpaceType::PSPACE>::SingleRange(size_t size) : mSize(size) { }
    
    int SingleRange<int,SpaceType::PSPACE>::get(size_t pos) const
    {
	return pos > mSize / 2 ? pos - mSize : pos;
    }
    
    size_t SingleRange<int,SpaceType::PSPACE>::getMeta(int metaPos) const
    {
	return metaPos < 0 ? metaPos + mSize : metaPos;
    }
    
    size_t SingleRange<int,SpaceType::PSPACE>::size() const
    {
	return mSize;
    }
    
    size_t SingleRange<int,SpaceType::PSPACE>::dim() const
    {
	return 1;
    }

    std::string SingleRange<int,SpaceType::PSPACE>::stringMeta(size_t pos) const
    {
	return std::to_string(get(pos));
    }

    std::vector<char> SingleRange<int,SpaceType::PSPACE>::data() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::PSPACE );
	h.metaSize = sizeof(size_t);
	h.multiple = 0;
	std::vector<char> out;
	out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	const char* scp = reinterpret_cast<const char*>(&mSize);
	out.insert(out.end(), scp, scp + h.metaSize);
	return out;
    }
    
    typename SingleRange<int,SpaceType::PSPACE>::IndexType SingleRange<int,SpaceType::PSPACE>::begin() const
    {
	SingleIndex<int,SpaceType::PSPACE> i( std::dynamic_pointer_cast<SingleRange<int,SpaceType::PSPACE> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    typename SingleRange<int,SpaceType::PSPACE>::IndexType SingleRange<int,SpaceType::PSPACE>::end() const
    {
	SingleIndex<int,SpaceType::PSPACE> i( std::dynamic_pointer_cast<SingleRange<int,SpaceType::PSPACE> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }
}

