
//
//#include "ranges/single_range.h"
#include "ranges/type_map.h"
#include "ranges/rheader.h"
#include "ranges/range_types/header.h"

#include "ranges/range_base.h"

#include <algorithm>

namespace MultiArrayTools
{

    //using namespace MultiArrayHelpers;

    template <class... Ranges>
    using STP = std::tuple<std::shared_ptr<Ranges>...>;

    typedef vector<std::shared_ptr<RangeBase> > RVEC;

    template <class... Ranges>
    inline bool compareSpaceTypes(const RVEC& rvec)
    {
	return RPackNum<sizeof...(Ranges)-1>::
	    template compareSpaceTypes<sizeof...(Ranges),Ranges...>(rvec);
    }

    template <class... Ranges>
    inline bool setFactory(const RVEC& rvec, std::shared_ptr<RangeFactoryBase>& fptr)
    {
	if(compareSpaceTypes<Ranges...>(rvec)) {
	    STP<Ranges...> stp;
	    RPackNum<sizeof...(Ranges)-1>::setSpace(rvec, stp);
	    fptr = std::make_shared<MultiRangeFactory<Ranges...> >(stp);
	    return true;
	}
	else {
	    return false;
	}
    }
    
    size_t indexId()
    {
	static size_t id = 0;
	++id;
	return id;
    }

    std::shared_ptr<RangeFactoryBase> mkMULTI(char const** dp, size_t metaSize)
    {
	std::shared_ptr<RangeFactoryBase> out = nullptr;
	RVEC rvec(metaSize);
	for(size_t i = 0; i != metaSize; ++i){
	    auto ff = createRangeFactory(dp);
	    rvec[i] = ff->create();
	}

	if(metaSize == 0){
	    assert(0);
	}	
	else if(metaSize == 1) {
#define register_multi1(TT0) if(setFactory<TT0>(rvec, out)) {} else
#include "ranges/multi_range_register.h"
#undef register_multi1
	    assert(0);
	}
	else if(metaSize == 2) {
#define register_multi2(TT0,TT1) if(setFactory<TT0,TT1>(rvec, out)) {} else
#include "ranges/multi_range_register.h"
#undef register_multi2
	    assert(0);
	}
	else if(metaSize == 3) {
#define register_multi3(TT0,TT1,TT2) if(setFactory<TT0,TT1,TT2>(rvec, out)) {} else
#include "ranges/multi_range_register.h"
#undef register_multi3
	    assert(0);
	}
	else if(metaSize == 4) {
#define register_multi4(TT0,TT1,TT2,TT3) if(setFactory<TT0,TT1,TT2,TT3>(rvec, out)) {} else
#include "ranges/multi_range_register.h"
#undef register_multi4
	    assert(0);
	}
	else {
	    assert(0);
	}
	
	return out;
    }

    std::shared_ptr<RangeFactoryBase> mkANONYMOUS(char const** dp, size_t metaSize)
    {
	std::shared_ptr<RangeFactoryBase> out = nullptr;
	auto arf = std::make_shared<AnonymousRangeFactory>();
	for(size_t i = 0; i != metaSize; ++i){
	    auto ff = createRangeFactory(dp);
	    arf->append( ff->create() );
	}
	out = arf;
	return out;
    }


    std::shared_ptr<RangeFactoryBase> mkANY(int metaType, size_t metaSize, char const** dp)
    {
        std::shared_ptr<RangeFactoryBase> out = nullptr;
        if(metaType == -1){
            assert(0);
        }
#define register_type(x) else if(x == metaType) { \
		    vector<TypeMap<x>::type> vd; \
		    metaCat(vd, *dp, metaSize); \
		    out = std::make_shared<SingleRangeFactory<TypeMap<x>::type, \
							      SpaceType::ANY> >(vd); }
#include "ranges/type_map.h"
#undef register_type
        else {
            assert(0);
        }
        return out;
    }

    std::shared_ptr<RangeFactoryBase> createSingleRangeFactory(const vector<char>*& d, int metaType, size_t size)
    {
        std::shared_ptr<RangeFactoryBase> out = nullptr;
        if(metaType == -1){
            assert(0);
        }
#define register_type(x) else if(x == metaType) { \
            vector<TypeMap<x>::type> vd(size);    \
            std::transform(d,d+size,vd.begin(), \
                           [](const vector<char>& c) \
                           { assert(c.size() == sizeof(TypeMap<x>::type)); \
                               return *reinterpret_cast<TypeMap<x>::type const*>(c.data()); }); \
            out = std::make_shared<SingleRangeFactory<TypeMap<x>::type, \
                                                      SpaceType::ANY> >(vd); }
#include "ranges/type_map.h"
#undef register_type
        else {
            assert(0);
        }
        return out;
    }
    
    std::shared_ptr<RangeFactoryBase> createRangeFactory(char const** dp)
    {
	DataHeader h = *reinterpret_cast<const DataHeader*>(*dp);
	*dp += sizeof(DataHeader);

	std::shared_ptr<RangeFactoryBase> out = nullptr;
	
	if(h.multiple != 0){
	    if(h.spaceType == static_cast<int>( SpaceType::ANY )) {
		// multi range
		out = mkMULTI(dp, h.metaSize);
	    }
	    else if(h.spaceType == static_cast<int>(  SpaceType::ANON ) ) {
		// anonymous range
		out = mkANONYMOUS(dp, h.metaSize);
	    }
	    else {
		assert(0);
	    }
	}
	else {
	    if(h.spaceType == static_cast<int>( SpaceType::ANY ) ) {
		// generic single range
                out = mkANY(h.metaType, h.metaSize, dp);
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
    
    std::intptr_t RangeBase::id() const
    {
        return reinterpret_cast<std::intptr_t>(this);
    }

} // end namespace MultiArrayTools
