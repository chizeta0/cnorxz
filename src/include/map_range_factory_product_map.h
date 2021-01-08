
#ifndef __map_range_factory_product_map_h__
#define __map_range_factory_product_map_h__

#include <memory>
#include <vector>
#include <map>
#include "ranges/rbase_def.h"
#include "mbase_def.h"

namespace MultiArrayTools
{
    class MapRangeFactoryProductMap
    {
    public:

	template <class ORType, class MapF, SpaceType XSTYPE, class... Ranges>
	friend class GenMapRangeFactory;
	
    private:
	static std::map<std::shared_ptr<RangeBase>,vector<std::intptr_t> > mAleadyCreated;
    };
    
}

#endif
