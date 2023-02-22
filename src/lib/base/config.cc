
#include "base/config.h"

namespace CNORXZ
{
    namespace Config
    {
	String version()
	{
	    return String(VERSION);
	}

	String commit()
	{
	    return String(GIT_COMMIT);
	}

	String flags()
	{
	    return String(CXX_FLAGS);
	}
    }
}
