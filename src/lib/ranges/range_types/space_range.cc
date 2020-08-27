
#include "ranges/rheader.h"
#include "ranges/x_to_string.h"
#include "ranges/ranges_header.cc.h"

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
    
    GenSingleRangeFactory<int,SpaceType::PSPACE,MUI>::GenSingleRangeFactory(size_t size)
    {
	// Quasi Singleton
	if(not mProd){
	    mProd = std::shared_ptr<oType>( new GenSingleRange<int,SpaceType::PSPACE,MUI>(size) );
	    setSelf();
	}
    }

    std::shared_ptr<RangeBase> GenSingleRangeFactory<int,SpaceType::PSPACE,MUI>::create()
    {
	return mProd;
    }
    
    /********************
     *   GenSingleRange    *
     ********************/

    GenSingleRange<int,SpaceType::PSPACE,MUI>::GenSingleRange(size_t size) : mSize(size) { }
    
    int GenSingleRange<int,SpaceType::PSPACE,MUI>::get(size_t pos) const
    {
	return pos > mSize / 2 ? pos - mSize : pos;
    }
    
    size_t GenSingleRange<int,SpaceType::PSPACE,MUI>::getMeta(int metaPos) const
    {
	return metaPos < 0 ? metaPos + mSize : metaPos;
    }
    
    size_t GenSingleRange<int,SpaceType::PSPACE,MUI>::size() const
    {
	return mSize;
    }
    
    size_t GenSingleRange<int,SpaceType::PSPACE,MUI>::dim() const
    {
	return 1;
    }

    SpaceType GenSingleRange<int,SpaceType::PSPACE,MUI>::spaceType() const
    {
	return SpaceType::PSPACE;
    }

    vector<size_t> GenSingleRange<int,SpaceType::PSPACE,MUI>::typeNum() const
    {
        return {NumTypeMap<int>::num()};
    }
    
    size_t GenSingleRange<int,SpaceType::PSPACE,MUI>::cmeta(char* target, size_t pos) const
    {
        *reinterpret_cast<int*>(target) = get(pos);
        return sizeof(int);
    }

    size_t GenSingleRange<int,SpaceType::PSPACE,MUI>::cmetaSize() const
    {
        return sizeof(int);
    }

    std::string GenSingleRange<int,SpaceType::PSPACE,MUI>::stringMeta(size_t pos) const
    {
	return std::to_string(get(pos));
    }

    vector<char> GenSingleRange<int,SpaceType::PSPACE,MUI>::data() const
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

    DataHeader GenSingleRange<int,SpaceType::PSPACE,MUI>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::PSPACE );
	h.metaSize = sizeof(size_t);
	h.multiple = 0;
        return h;
    }
    
    typename GenSingleRange<int,SpaceType::PSPACE,MUI>::IndexType GenSingleRange<int,SpaceType::PSPACE,MUI>::begin() const
    {
	GenSingleIndex<int,SpaceType::PSPACE,MUI> i( std::dynamic_pointer_cast<GenSingleRange<int,SpaceType::PSPACE,MUI> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    typename GenSingleRange<int,SpaceType::PSPACE,MUI>::IndexType GenSingleRange<int,SpaceType::PSPACE,MUI>::end() const
    {
	GenSingleIndex<int,SpaceType::PSPACE,MUI> i( std::dynamic_pointer_cast<GenSingleRange<int,SpaceType::PSPACE,MUI> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }
}

