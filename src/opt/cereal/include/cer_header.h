// -*- C++ -*-
/**

   @file opt/cereal/include/cer_header.h
   @brief CNORXZ Cereal data header declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_cereal_header_h__
#define __cxz_cereal_header_h__

#include "base/base.h"
#include "cer_base.h"

namespace CNORXZ
{
    namespace cer
    {
	/** ****
	    Cereal data header struct.
	 */
	struct Header
	{
	    String version; /**< CNORXZ version. */
	    String commit; /**< CNORXZ git commit. */
	    ContentType content; /**< Content type. */
	};

	/** Create header.
	    @param content Content type.
	 */
	inline Header mkHeader(const ContentType content)
	{
	    Header o;
	    o.version = CNORXZ::Config::version();
	    o.commit = CNORXZ::Config::commit();
	    o.content = content;
	    return o;
	}

	/** Serialze header, store in archive.
	    @param ar Target archive.
	    @param h Input header.
	    @param version Version.
	 */
	template <class Archive>
	void save(Archive& ar, const Header& h, const std::uint32_t version);

	/** Deserialze header from archive.
	    @param ar Source archive.
	    @param h Target header reference.
	    @param version Version.
	 */
	template <class Archive>
	void load(Archive& ar, Header& h, const std::uint32_t version);

    }
}

#endif
