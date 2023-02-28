
#ifndef __cxz_cereal_utilities_h__
#define __cxz_cereal_utilities_h__

#include <istream>
#include <ostream>
#include "base/base.h"
#include "cer_base.h"
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>

namespace CNORXZ
{
    namespace cer
    {

	template <Format F>
	struct OutputFormatMap
	{};

	template <>
	struct OutputFormatMap<Format::BINARY>
	{ typedef cereal::BinaryOutputArchive type; };

	template <>
	struct OutputFormatMap<Format::JSON>
	{ typedef cereal::JSONOutputArchive type; };

	template <>
	struct OutputFormatMap<Format::XML>
	{ typedef cereal::XMLOutputArchive type; };

	template <Format F>
	struct InputFormatMap
	{};

	template <>
	struct InputFormatMap<Format::BINARY>
	{ typedef cereal::BinaryInputArchive type; };

	template <>
	struct InputFormatMap<Format::JSON>
	{ typedef cereal::JSONInputArchive type; };

	template <>
	struct InputFormatMap<Format::XML>
	{ typedef cereal::XMLInputArchive type; };

    	template <Format F, typename T>
	void write(std::ostream& os, const MArray<T>& data);

    	template <Format F, typename T>
	void read(std::istream& is, MArray<T>& data);

    	template <Format F, typename T>
	void writeFile(const String& name, const MArray<T>& data);

    	template <Format F, typename T>
	void readFile(const String& name, MArray<T>& data);
    }
}

#endif
