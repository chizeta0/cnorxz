
#ifndef __cxz_cer_base_h__
#define __cxz_cer_base_h__

#define CXZ_CEREAL_FORMAT_VERION 1u

namespace CNORXZ
{
    namespace cer
    {
	enum class Format {
	    BINARY = 1u,
	    JSON = 2u,
	    XML = 3u
	};
	
	enum class ContentType {
	    ARRAY = 1u,
	    MAP = 2u
	};
    }
}

#endif
