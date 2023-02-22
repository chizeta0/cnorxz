
#ifndef __cxz_config_h__
#define __cxz_config_h__

#include <cstdlib>
#include "base/types.h"

namespace CNORXZ
{
    namespace Config
    {
	String version();

	String commit();

	String flags();
    }
}

#endif
