
//
//#include "ranges/single_range.h"
#include "ranges/type_map.h"
#include "ranges/rheader.h"
#include "ranges/range_types/header.h"

#include "ranges/range_base.h"

namespace MultiArrayTools
{

    //using namespace MultiArrayHelpers;

    size_t indexId()
    {
	static size_t id = 0;
	++id;
	return id;
    }

    // !!!!!
    std::shared_ptr<RangeFactoryBase> mkMULTI(char** dp)
    {
	return nullptr;
    }

    // !!!!!
    std::shared_ptr<RangeFactoryBase> mkANONYMOUS(char** dp)
    {
	return nullptr;
    }

    std::shared_ptr<RangeFactoryBase> createRangeFactory(char** dp)
    {
	DataHeader h = *reinterpret_cast<const DataHeader*>(*dp);
	*dp += sizeof(DataHeader);

	std::shared_ptr<RangeFactoryBase> out = nullptr;
	
	if(h.multiple != 0){
	    if(h.spaceType == static_cast<int>( SpaceType::ANY )) {
		// multi range
		out = mkMULTI(dp);
	    }
	    else if(h.spaceType == static_cast<int>(  SpaceType::ANON ) ) {
		// anonymous range
		out = mkANONYMOUS(dp);
	    }
	    else {
		assert(0);
	    }
	}
	else {
	    VCHECK(h.spaceType);
	    if(h.spaceType == static_cast<int>( SpaceType::ANY ) ) {
		VCHECK(h.metaType)
		// generic single range
		if(h.metaType == -1){
		    assert(0);
		}
#define register_type(x) else if(x == h.metaType) {\
		    VCHECK(x);\
		    std::vector<TypeMap<x>::type> vd;\
		    metaCat(vd, *dp, h.metaSize); \
		    out = std::make_shared<SingleRangeFactory<TypeMap<x>::type, \
							      SpaceType::ANY> >(vd); }
#include "ranges/type_map.h"
#undef register_type
		else {
		    assert(0);
		}
		
	    }
	    else if(h.spaceType == static_cast<int>( SpaceType::NONE ) ) {
		// classic range
		size_t size = *reinterpret_cast<const size_t*>(*dp);
		out = std::make_shared<SingleRangeFactory<size_t,SpaceType::NONE> >(size);
	    }
#define include_range_type(x,n) else if(h.spaceType == static_cast<int>( SpaceType::x ) ) { \
		out = mk##x(*dp, h.metaSize); }
#include "ranges/range_types/header.h"
#undef inlcude_range_type
	    else {
		assert(0);
	    }
	    *dp += h.metaSize;
	}
	return out;
    }
    
    /*************************
     *   RangeFactoryBase    *
     *************************/
    
    void RangeFactoryBase::setSelf()
    {
	mProd->mThis = mProd;
    }
    
    /******************
     *   RangeBase    *
     ******************/

    bool RangeBase::operator==(const RangeBase& in) const
    {
	return this == &in;
    }
    
    bool RangeBase::operator!=(const RangeBase& in) const
    {
	return this != &in;
    }


} // end namespace MultiArrayTools
