
#ifndef __range_save_load_h__
#define __range_save_load_h__

#include "base/base.h"

namespace CNORXZ
{
    namespace cer
    {
	// use this function to register the range in the map of the range factories
	// (otherwise there might be more than one instance of the same range)
	template <class Archive>
	RangePtr save_load(Archive& ar, const String& name, RangePtr& r);
    }
}

#endif
