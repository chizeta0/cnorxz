
#ifndef __range_save_load_h__
#define __range_save_load_h__

#include "base/base.h"

namespace CNORXZ
{
    namespace cer
    {
	template <class Archive>
	RangePtr save_load(Archive& ar, RangePtr& r);
    }
}

#endif
