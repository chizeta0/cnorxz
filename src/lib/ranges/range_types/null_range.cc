
#include "ranges/rheader.h"
#include "ranges/x_to_string.h"

namespace MultiArrayTools
{
    std::shared_ptr<NullRF> mkNUL(const char* dp, size_t size)
    {
	return std::make_shared<NullRF>();
    }
	
    /********************
     *   GenSingleRange    *
     ********************/

    std::shared_ptr<GenSingleRange<size_t,SpaceType::NUL,0>>
    GenSingleRangeFactory<size_t,SpaceType::NUL,0>::mRInstance = nullptr;
    
    std::shared_ptr<NullRange> nullr()
    {
	static NullRF nrf; // just that mRInstance is initialized
	static auto nr = std::dynamic_pointer_cast<NullRange>( nrf.create() );
	return NullRF::mRInstance;
    }

    std::shared_ptr<NullIndex> nulli()
    {
	return std::make_shared<NullIndex>(nullr());
    }

    GenSingleRangeFactory<size_t,SpaceType::NUL,0>::GenSingleRangeFactory()
    {
	// Singleton
	if(not mRInstance){
	    if(not mProd){
		mProd = std::shared_ptr<oType>( new GenSingleRange<size_t,SpaceType::NUL,0>() );
		setSelf();
	    }
	    mRInstance = std::dynamic_pointer_cast<NullRange>( mProd );
	} else {
	    mProd = mRInstance;
	}
    }

    std::shared_ptr<RangeBase> GenSingleRangeFactory<size_t,SpaceType::NUL,0>::create()
    {
	return mProd;
    }
    
    /********************
     *   GenSingleRange    *
     ********************/
    
    size_t GenSingleRange<size_t,SpaceType::NUL,0>::get(size_t pos) const
    {
	return 0;
    }
    
    size_t GenSingleRange<size_t,SpaceType::NUL,0>::getMeta(size_t metapos) const
    {
	return 0;
    }
    
    size_t GenSingleRange<size_t,SpaceType::NUL,0>::size() const
    {
	return 1;
    }
    
    size_t GenSingleRange<size_t,SpaceType::NUL,0>::dim() const
    {
	return 1;
    }

    SpaceType GenSingleRange<size_t,SpaceType::NUL,0>::spaceType() const
    {
	return SpaceType::NUL;
    }

    size_t GenSingleRange<size_t,SpaceType::NUL,0>::typeNum() const
    {
        return NumTypeMap<size_t>::num;
    }
    
    size_t GenSingleRange<size_t,SpaceType::NUL,0>::cmeta(char* target, size_t pos) const
    {
        if(target){
            *reinterpret_cast<size_t*>(target) = 0;
        }
        return sizeof(size_t);
    }

    std::string GenSingleRange<size_t,SpaceType::NUL,0>::stringMeta(size_t pos) const
    {
	return std::to_string(get(pos));
    }

    vector<char> GenSingleRange<size_t,SpaceType::NUL,0>::data() const
    {
	DataHeader h = dataHeader(); 
	vector<char> out;
	out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	return out;
    }

    DataHeader GenSingleRange<size_t,SpaceType::NUL,0>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::NUL );
	h.metaSize = 0;
	h.multiple = 0;
        return h;
    }
   
    typename GenSingleRange<size_t,SpaceType::NUL,0>::IndexType GenSingleRange<size_t,SpaceType::NUL,0>::begin() const
    {
	GenSingleIndex<size_t,SpaceType::NUL,0> i( std::dynamic_pointer_cast<GenSingleRange<size_t,SpaceType::NUL,0> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    typename GenSingleRange<size_t,SpaceType::NUL,0>::IndexType GenSingleRange<size_t,SpaceType::NUL,0>::end() const
    {
	GenSingleIndex<size_t,SpaceType::NUL,0> i( std::dynamic_pointer_cast<GenSingleRange<size_t,SpaceType::NUL,0> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }

    // put this in the interface class !!!
    /*
    std::shared_ptr<VIWB> GenSingleRange<size_t,SpaceType::NUL,0>::index() const
    {
	typedef IndexWrapper<IndexType> IW;
	return std::make_shared<IW>
            ( std::make_shared<IndexType>
	      ( std::dynamic_pointer_cast<GenSingleRange<size_t,SpaceType::NUL,0> >
		( std::shared_ptr<RangeBase>( RB::mThis ) ) ) );
    }
    */
}

