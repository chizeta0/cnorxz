
#ifndef __cxz_multi_range_factory_product_map_h__
#define __cxz_multi_range_factory_product_map_h__

#include <memory>
#include <vector>
#include <map>
#include "ranges/rbase_def.h"

namespace CNORXZ
{
    class MultiRangeFactoryProductMap
    {
    public:

	template <class... Ranges>
	friend class MultiRangeFactory;
	
    private:
	static std::map<std::shared_ptr<RangeBase>,vector<std::intptr_t> > mAleadyCreated;
    };
    
}

#endif
