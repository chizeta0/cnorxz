
#ifndef __cxz_cereal_header_cc_h__
#define __cxz_cereal_header_cc_h__

#include "cer_header.h"

namespace CNORXZ
{
    namespace cer
    {
	template <class Archive>
	void save(Archive& ar, const Header& h, const std::uint32_t version)
	{
	    CXZ_ASSERT(version == 1u, "format version = " << version << " not supported");
	    ar(cereal::make_nvp("version",h.version));
	    ar(cereal::make_nvp("commit",h.commit));
	    ar(cereal::make_nvp("content",static_cast<SizeT>(h.content)));
	}

	template <class Archive>
	void load(Archive& ar, Header& h, const std::uint32_t version)
	{
	    CXZ_ASSERT(version == 1u, "format version = " << version << " not supported");
	    ar(cereal::make_nvp("version",h.version));
	    ar(cereal::make_nvp("commit",h.commit));
	    SizeT hc;
	    ar(cereal::make_nvp("content",hc));
	    h.content = static_cast<ContentType>(hc);
	}

    }
}

#endif
