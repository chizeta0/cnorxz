// -*- C++ -*-
/**
   
   @file include/base/config.h
   @brief runtime config functions

   Declare functions returning version and configuration infos

   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_config_h__
#define __cxz_config_h__

#include <cstdlib>
#include "base/types.h"

namespace CNORXZ
{
    namespace Config
    {
	/** return cnorxz version */
	String version();

	/** return git hash */
	String commit();

	/** return compile flags */
	String flags();
    }
}

#endif
