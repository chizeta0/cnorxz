
#include "multi_array_header.h"
#include "high_level_operation.h"

namespace MultiArrayTools
{
    std::shared_ptr<DynamicIndex> mkSubSpaceX(const std::shared_ptr<DynamicIndex>& di,
					      size_t max)
    {
	auto& o = di->range()->orig();
	vector<std::shared_ptr<RangeBase>> ox(o.begin(),o.begin()+max);
	DynamicRangeFactory drf(ox);
	auto dr = createExplicit(drf);
	auto odi = getIndex(dr);
	vector<std::shared_ptr<IndexW>> iv;
	iv.reserve(max);
	for(size_t i = 0; i != max; ++i){
	    iv.push_back(di->getP(i));
	}
	(*odi)(iv);
	return odi;
    }

}
