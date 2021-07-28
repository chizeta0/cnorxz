
#include "ranges/rheader.h"
#include "ranges/x_to_string.h"
#include "ranges/ranges_header.cc.h"

namespace CNORXZ
{
    /********************
     *   GenSingleRange    *
     ********************/
    
    GenSingleRangeFactory<size_t,SpaceType::NONE,MUI>::GenSingleRangeFactory(size_t size)
    {
	// Quasi Singleton
	if(not mProd){
	    mProd = std::shared_ptr<oType>( new GenSingleRange<size_t,SpaceType::NONE,MUI>(size) );
	    setSelf();
	}
    }

    std::shared_ptr<RangeBase> GenSingleRangeFactory<size_t,SpaceType::NONE,MUI>::create()
    {
	return mProd;
    }
    
    /********************
     *   GenSingleRange    *
     ********************/

    GenSingleRange<size_t,SpaceType::NONE,MUI>::GenSingleRange(size_t size) : mSize(size) {}
    
    size_t GenSingleRange<size_t,SpaceType::NONE,MUI>::get(size_t pos) const
    {
	return pos;
    }
    
    size_t GenSingleRange<size_t,SpaceType::NONE,MUI>::getMeta(size_t metaPos) const
    {
	return metaPos;
    }
    
    size_t GenSingleRange<size_t,SpaceType::NONE,MUI>::size() const
    {
	return mSize;
    }
    
    size_t GenSingleRange<size_t,SpaceType::NONE,MUI>::dim() const
    {
	return 1;
    }

    SpaceType GenSingleRange<size_t,SpaceType::NONE,MUI>::spaceType() const
    {
	return SpaceType::NONE;
    }

    vector<size_t> GenSingleRange<size_t,SpaceType::NONE,MUI>::typeNum() const
    {
        return {NumTypeMap<size_t>::num()};
    }
    
    size_t GenSingleRange<size_t,SpaceType::NONE,MUI>::cmeta(char* target, size_t pos) const
    {
        *reinterpret_cast<size_t*>(target) = pos;
        return sizeof(size_t);
    }

    size_t GenSingleRange<size_t,SpaceType::NONE,MUI>::cmetaSize() const
    {
        return sizeof(size_t);
    }

    std::string GenSingleRange<size_t,SpaceType::NONE,MUI>::stringMeta(size_t pos) const
    {
	return std::to_string(get(pos));
    }

    vector<char> GenSingleRange<size_t,SpaceType::NONE,MUI>::data() const
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

    DataHeader GenSingleRange<size_t,SpaceType::NONE,MUI>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::NONE );
	h.metaSize = sizeof(size_t);
	h.multiple = 0;
        return h;
    }

    
    typename GenSingleRange<size_t,SpaceType::NONE,MUI>::IndexType GenSingleRange<size_t,SpaceType::NONE,MUI>::begin() const
    {
	GenSingleIndex<size_t,SpaceType::NONE,MUI> i( std::dynamic_pointer_cast<GenSingleRange<size_t,SpaceType::NONE,MUI> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    typename GenSingleRange<size_t,SpaceType::NONE,MUI>::IndexType GenSingleRange<size_t,SpaceType::NONE,MUI>::end() const
    {
	GenSingleIndex<size_t,SpaceType::NONE,MUI> i( std::dynamic_pointer_cast<GenSingleRange<size_t,SpaceType::NONE,MUI> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }
}

