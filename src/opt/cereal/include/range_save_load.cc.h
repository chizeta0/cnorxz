
#ifndef __range_save_load_cc_h__
#define __range_save_load_cc_h__

#include "ranges/range_base.h"
#include "range_save_load.h"
#include <cereal/cereal.hpp>

namespace CNORXZ
{
    namespace cer
    {
	template <class Archive>
	RangePtr save_load(Archive& ar, const String& name, RangePtr& r)
	{
	    ar(cereal::make_nvp(name, r));
	    return r = RangeFactoryBase::getRegistered(r->type(), r);
	}
    }
}


#endif
