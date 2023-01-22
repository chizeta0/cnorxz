
#ifndef __cxz_h5_table_cc_h__
#define __cxz_h5_table_cc_h__

#include "h5_table.h"

namespace CNORXZ
{
    namespace hdf5
    {
	template <class F>
	decltype(auto) iterRecords(F&& f) const
	{

	}

	template <typename... Ts>
	Table& appendRecord(const Tuple<Ts...>& t) const
	{
	    
	}
    }
}

#endif
