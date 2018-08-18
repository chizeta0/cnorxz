
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

	template <class MapF, class... Ranges>
	friend class MapRangeFactory;
	
    private:
	static std::map<std::shared_ptr<RangeBase>,std::vector<std::intptr_t> > mAleadyCreated;
    };
    
}

#endif
