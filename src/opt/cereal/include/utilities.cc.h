// -*- C++ -*-
/**

   @file opt/cereal/include/utilities.cc.h
   @brief CNORXZ Cereal utilities implementation.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

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
	    CXZ_ASSERT(os.good(), "could not open output file " << name);
	    write<F>(os, data);
	    CXZ_ASSERT(os.good(), "an error occurred while writing data to file " << name);
	    os.close();
	}

    	template <Format F, typename T>
	void readFile(const String& name, MArray<T>& data)
	{
	    std::fstream is(name, std::ios::binary | std::ios::in);
	    CXZ_ASSERT(is.good(), "could not open input file " << name);
	    read<F>(is, data);
	    //CXZ_ASSERT(not is.fail(), "an error occurred reading data from file " << name);
	    is.close();
	}
	
    }
}

#endif
