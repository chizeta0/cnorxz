
#ifndef __cxz_cereal_header_h__
#define __cxz_cereal_header_h__

#include "base/base.h"
#include "cer_base.h"

namespace CNORXZ
{
    namespace cer
    {
	struct Header
	{
	    String version;
	    String commit;
	    ContentType content;
	};

	inline Header mkHeader(const ContentType content)
	{
	    Header o;
	    o.version = CNORXZ::Config::version();
	    o.commit = CNORXZ::Config::commit();
	    o.content = content;
	    return o;
	}
	
	template <class Archive>
	void save(Archive& ar, const Header& h, const std::uint32_t version);

	template <class Archive>
	void load(Archive& ar, Header& h, const std::uint32_t version);

    }
}

#endif
