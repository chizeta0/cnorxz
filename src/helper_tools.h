
#ifndef __helper_tools_h__
#define __helper_tools_h__

#include "base_def.h"

namespace MultiArrayTools
{

    template <class RangeType>
    auto getIndex(std::shared_ptr<RangeType> range)
	-> std::shared_ptr<typename RangeType::IndexType>;
    
}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    template <class RangeType>
    auto getIndex(std::shared_ptr<RangeType> range)
	-> std::shared_ptr<typename RangeType::IndexType>
    {
	return std::dynamic_pointer_cast<typename RangeType::IndexType>( range->index() );
    }
}

#endif
