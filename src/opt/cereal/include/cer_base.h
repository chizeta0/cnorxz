// -*- C++ -*-
/**

   @file opt/cereal/include/cer_base.h
   @brief CNORXZ Cereal basic types declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_cer_base_h__
#define __cxz_cer_base_h__

#define CXZ_CEREAL_FORMAT_VERION 1u /**< CNORXZ specific cereal format version. */

namespace CNORXZ
{
    namespace cer
    {
	/** ****
	    Archive format enum class.
	 */
	enum class Format {
	    BINARY = 1u,
	    JSON = 2u,
	    XML = 3u
	};
	
	/** ****
	    Content type enum class.
	 */
	enum class ContentType {
	    ARRAY = 1u,
	    MAP = 2u
	};
    }
}

#endif
