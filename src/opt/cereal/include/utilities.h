
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

	template <typename T>
	void writeBINARYFile(const String& name, const MArray<T>& data) { writeFile<Format::BINARY>(name, data); }

    	template <typename T>
	void readBINARYFile(const String& name, MArray<T>& data) { readFile<Format::BINARY>(name, data); }

	template <typename T>
	void writeJSONFile(const String& name, const MArray<T>& data) { writeFile<Format::JSON>(name, data); }

    	template <typename T>
	void readJSONFile(const String& name, MArray<T>& data) { readFile<Format::JSON>(name, data); }

	template <typename T>
	void writeXMLFile(const String& name, const MArray<T>& data) { writeFile<Format::XML>(name, data); }

    	template <typename T>
	void readXMLFile(const String& name, MArray<T>& data) { readFile<Format::XML>(name, data); }
    }
}

#endif
