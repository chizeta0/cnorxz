
#include "ranges/rheader.h"
#include "ranges/x_to_string.h"

namespace MultiArrayTools
{
    std::shared_ptr<SpinRF> mkSPIN(const char* dp, size_t size)
    {
	return std::make_shared<SpinRF>();
    }
    
    /********************
     *   GenSingleRange    *
     ********************/
    
    GenSingleRangeFactory<size_t,SpaceType::SPIN,4>::GenSingleRangeFactory()
    {
	// Quasi Singleton
	if(not mProd){
	    mProd = std::shared_ptr<oType>( new GenSingleRange<size_t,SpaceType::SPIN,4>() );
	    setSelf();
	}
    }

    std::shared_ptr<RangeBase> GenSingleRangeFactory<size_t,SpaceType::SPIN,4>::create()
    {
	return mProd;
    }
    
    /********************
     *   GenSingleRange    *
     ********************/
    
    size_t GenSingleRange<size_t,SpaceType::SPIN,4>::get(size_t pos) const
    {
	return pos;
    }
    
    size_t GenSingleRange<size_t,SpaceType::SPIN,4>::getMeta(size_t metaPos) const
    {
	return metaPos;
    }
    
    size_t GenSingleRange<size_t,SpaceType::SPIN,4>::size() const
    {
	return mSpinNum;
    }
    
    size_t GenSingleRange<size_t,SpaceType::SPIN,4>::dim() const
    {
	return 1;
    }

    SpaceType GenSingleRange<size_t,SpaceType::SPIN,4>::spaceType() const
    {
	return SpaceType::SPIN;
    }

    size_t GenSingleRange<size_t,SpaceType::SPIN,4>::typeNum() const
    {
        return NumTypeMap<size_t>::num;
    }
    
    size_t GenSingleRange<size_t,SpaceType::SPIN,4>::cmeta(char* target, size_t pos) const
    {
        if(target){
            *reinterpret_cast<size_t*>(target) = pos;
        }
        return sizeof(size_t);
    }

    std::string GenSingleRange<size_t,SpaceType::SPIN,4>::stringMeta(size_t pos) const
    {
	return std::to_string(get(pos));
    }

    vector<char> GenSingleRange<size_t,SpaceType::SPIN,4>::data() const
    {
	DataHeader h = dataHeader();
	vector<char> out;
	out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	return out;
    }
    
    DataHeader GenSingleRange<size_t,SpaceType::SPIN,4>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::SPIN );
	h.metaSize = 0;
	h.multiple = 0;
        return h;
    }    
    
    typename GenSingleRange<size_t,SpaceType::SPIN,4>::IndexType GenSingleRange<size_t,SpaceType::SPIN,4>::begin() const
    {
	GenSingleIndex<size_t,SpaceType::SPIN,4> i( std::dynamic_pointer_cast<GenSingleRange<size_t,SpaceType::SPIN,4> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    typename GenSingleRange<size_t,SpaceType::SPIN,4>::IndexType GenSingleRange<size_t,SpaceType::SPIN,4>::end() const
    {
	GenSingleIndex<size_t,SpaceType::SPIN,4> i( std::dynamic_pointer_cast<GenSingleRange<size_t,SpaceType::SPIN,4> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }

    // put this in the interface class !!!
    /*
    std::shared_ptr<VIWB> GenSingleRange<size_t,SpaceType::SPIN,4>::index() const
    {
	typedef IndexWrapper<IndexType> IW;
	return std::make_shared<IW>
            ( std::make_shared<IndexType>
	      ( std::dynamic_pointer_cast<GenSingleRange<size_t,SpaceType::SPIN,4> >
		( std::shared_ptr<RangeBase>( RB::mThis ) ) ) );
    }
    */
}

