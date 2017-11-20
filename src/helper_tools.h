
#include "base_def.h"

namespace MultiArrayTools
{

    template <class RangeType>
    auto getIndex(std::shared_ptr<RangeType> range)
	-> std::shared_ptr<typename RangeType::IndexType>;
    
}

// =====================
// ===    C O D E    ===
// =====================

namespace MultiArrayTools
{
    template <class RangeType>
    auto getIndex(std::shared_ptr<RangeType> range)
	-> std::shared_ptr<typename RangeType::IndexType>
    {
	return std::dynamic_pointer_cast<typename RangeType::IndexType>( range->index() );
    }
}
