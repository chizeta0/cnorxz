
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

    template class OperationRoot<double,CR,DynamicRange>;
    template class OperationRoot<double,DynamicRange>;

    template class HighLevelOpHolder<OpCD>;
    template class HighLevelOpHolder<OpD>;

    template class HighLevelOpBase<OpCD>;
    template class HighLevelOpBase<OpD>;
    template class HighLevelOpRoot<OpCD>;
    template class HighLevelOpRoot<OpD>;

    template HighLevelOpHolder<OpCD> mkHLO(const OpCD& op);
    template HighLevelOpHolder<OpD> mkHLO(const OpD& op);
    template HighLevelOpHolder<OpCD> mkHLOV(double val);
    template HighLevelOpHolder<OpD> mkHLOV(double val);

}
