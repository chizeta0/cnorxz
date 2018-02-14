
#ifndef __utils_h__
#define __utils_h__

#include <ostream>
#include "pack_num.h"

namespace MultiArrayHelper
{

    using namespace MultiArrayTools;

    template <typename... T>
    std::ostream& operator<<(std::ostream& out, const std::tuple<T...>& tp);

    /*==================*
     *   TEMPLATE CODE  *
     *==================*/
    
    template <typename... T>
    std::ostream& operator<<(std::ostream& out, const std::tuple<T...>& tp)
    {
	PackNum<sizeof...(T)-1>::printTuple(out, tp);
	return out;
    }


} // end namespace MultiArrayHelper

#endif
