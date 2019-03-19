
#include "ranges/rheader.h"
#include "ranges/x_to_string.h"

namespace MultiArrayTools
{
    std::shared_ptr<PSpaceRF> mkPSPACE(const char* dp, size_t size)
    {
	size_t max = *reinterpret_cast<const size_t*>(dp);
	return std::make_shared<PSpaceRF>(max);
    }
    
    /********************
     *   GenSingleRange    *
     ********************/
    
    GenSingleRangeFactory<int,SpaceType::PSPACE,-1>::GenSingleRangeFactory(size_t size)
    {
	// Quasi Singleton
	if(not mProd){
	    mProd = std::shared_ptr<oType>( new GenSingleRange<int,SpaceType::PSPACE,-1>(size) );
	    setSelf();
	}
    }

    std::shared_ptr<RangeBase> GenSingleRangeFactory<int,SpaceType::PSPACE,-1>::create()
    {
	return mProd;
    }
    
    /********************
     *   GenSingleRange    *
     ********************/

    GenSingleRange<int,SpaceType::PSPACE,-1>::GenSingleRange(size_t size) : mSize(size) { }
    
    int GenSingleRange<int,SpaceType::PSPACE,-1>::get(size_t pos) const
    {
	return pos > mSize / 2 ? pos - mSize : pos;
    }
    
    size_t GenSingleRange<int,SpaceType::PSPACE,-1>::getMeta(int metaPos) const
    {
	return metaPos < 0 ? metaPos + mSize : metaPos;
    }
    
    size_t GenSingleRange<int,SpaceType::PSPACE,-1>::size() const
    {
	return mSize;
    }
    
    size_t GenSingleRange<int,SpaceType::PSPACE,-1>::dim() const
    {
	return 1;
    }

    SpaceType GenSingleRange<int,SpaceType::PSPACE,-1>::spaceType() const
    {
	return SpaceType::PSPACE;
    }

    vector<size_t> GenSingleRange<int,SpaceType::PSPACE,-1>::typeNum() const
    {
        return {NumTypeMap<int>::num};
    }
    
    size_t GenSingleRange<int,SpaceType::PSPACE,-1>::cmeta(char* target, size_t pos) const
    {
        *reinterpret_cast<int*>(target) = get(pos);
        return sizeof(int);
    }

    size_t GenSingleRange<int,SpaceType::PSPACE,-1>::cmetaSize() const
    {
        return sizeof(int);
    }

    std::string GenSingleRange<int,SpaceType::PSPACE,-1>::stringMeta(size_t pos) const
    {
	return std::to_string(get(pos));
    }

    vector<char> GenSingleRange<int,SpaceType::PSPACE,-1>::data() const
    {
	DataHeader h = dataHeader();
	vector<char> out;
	out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	const char* scp = reinterpret_cast<const char*>(&mSize);
	out.insert(out.end(), scp, scp + h.metaSize);
	return out;
    }

    DataHeader GenSingleRange<int,SpaceType::PSPACE,-1>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::PSPACE );
	h.metaSize = sizeof(size_t);
	h.multiple = 0;
        return h;
    }
    
    typename GenSingleRange<int,SpaceType::PSPACE,-1>::IndexType GenSingleRange<int,SpaceType::PSPACE,-1>::begin() const
    {
	GenSingleIndex<int,SpaceType::PSPACE,-1> i( std::dynamic_pointer_cast<GenSingleRange<int,SpaceType::PSPACE,-1> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    typename GenSingleRange<int,SpaceType::PSPACE,-1>::IndexType GenSingleRange<int,SpaceType::PSPACE,-1>::end() const
    {
	GenSingleIndex<int,SpaceType::PSPACE,-1> i( std::dynamic_pointer_cast<GenSingleRange<int,SpaceType::PSPACE,-1> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }
}

