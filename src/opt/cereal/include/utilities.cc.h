
#ifndef __cxz_cereal_utilities_cc_h__
#define __cxz_cereal_utilities_cc_h__

#include <fstream>
#include "utilities.h"
#include "cer_header.h"

namespace CNORXZ
{
    namespace cer
    {
	
    	template <Format F, typename T>
	void write(std::ostream& os, const MArray<T>& data)
	{
	    typedef typename OutputFormatMap<F>::type OArchive;
	    OArchive ar(os);

	    auto h = mkHeader(ContentType::ARRAY);
	    ar(cereal::make_nvp("cnorxz",h));
	    ar(cereal::make_nvp("array",data));
	}
	
    	template <Format F, typename T>
	void read(std::istream& is, MArray<T>& data)
	{
	    typedef typename InputFormatMap<F>::type IArchive;
	    IArchive ar(is);

	    Header h;
	    ar(cereal::make_nvp("cnorxz",h));
	    CXZ_ASSERT(h.content == ContentType::ARRAY, "expected Array (type = "
		       << static_cast<SizeT>(ContentType::ARRAY) << "), got "
		       << static_cast<SizeT>(h.content));
	    ar(cereal::make_nvp("array",data));
	}

    	template <Format F, typename T>
	void writeFile(const String& name, const MArray<T>& data)
	{
	    std::fstream os(name, std::ios::binary | std::ios::out);
	    write<F>(os, data);
	    os.close();
	}

    	template <Format F, typename T>
	void readFile(const String& name, MArray<T>& data)
	{
	    std::fstream is(name, std::ios::binary | std::ios::in);
	    read<F>(is, data);
	    is.close();
	}
	
    }
}

#endif
