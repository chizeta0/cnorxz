// -*- C++ -*-
/**
   
   @file lib/base/config.cc
   @brief Runtime config functions implementations.

   Declare functions returning version and configuration infos.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

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

	String definitions()
	{
	    String o;
#ifdef HAVE_CEREAL
	    o += "HAVE_CEREAL ";
#endif
#ifdef CXZ_HAVE_AVX
	    o += "CXZ_HAVE_AVX ";
#endif
	    return o;
	}
    }
}
