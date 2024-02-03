// -*- C++ -*-
/**

   @file opt/cereal/include/utilities.h
   @brief CNORXZ Cereal utilities declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

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
	/** ****
	    Output archive type trait.
	    @tparam F Format enum.
	 */
	template <Format F>
	struct OutputFormatMap
	{};

	/** Output archive for binary format. */
	template <>
	struct OutputFormatMap<Format::BINARY>
	{ typedef cereal::BinaryOutputArchive type; /**< archive type. */ };

	/** Output archive for json format. */
	template <>
	struct OutputFormatMap<Format::JSON>
	{ typedef cereal::JSONOutputArchive type; /**< archive type. */ };

	/** Output archive for xml format. */
	template <>
	struct OutputFormatMap<Format::XML>
	{ typedef cereal::XMLOutputArchive type; /**< archive type. */ };

	/** ****
	    Input archive type trait.
	    @tparam F Format enum.
	 */
	template <Format F>
	struct InputFormatMap
	{};

	/** Input archive for binary format. */
	template <>
	struct InputFormatMap<Format::BINARY>
	{ typedef cereal::BinaryInputArchive type; /**< archive type. */ };

	/** Input archive for json format. */
	template <>
	struct InputFormatMap<Format::JSON>
	{ typedef cereal::JSONInputArchive type; /**< archive type. */ };

	/** Input archive for xml format. */
	template <>
	struct InputFormatMap<Format::XML>
	{ typedef cereal::XMLInputArchive type; /**< archive type. */ };

	/** Serialize MArray to stream
	    @tparam F Format enum class [BINARY,JSON,XML].
	    @tparam T Array element value type.
	    @param os Output stream.
	    @param data Array to be serialized.
	 */
    	template <Format F, typename T>
	void write(std::ostream& os, const MArray<T>& data);

	/** Deserialize MArray from stream
	    @tparam F Format enum class [BINARY,JSON,XML].
	    @tparam T Array element value type.
	    @param is Input stream.
	    @param data Target array.
	 */
    	template <Format F, typename T>
	void read(std::istream& is, MArray<T>& data);

	/** Serialize MArray and write to file.
	    @tparam F Format enum class [BINARY,JSON,XML].
	    @tparam T Array element value type.
	    @param name File name.
	    @param data Array to be saved.
	 */
    	template <Format F, typename T>
	void writeFile(const String& name, const MArray<T>& data);

	/** Read and deserialize MArray from cereal compatibel file.
	    @tparam F Format enum class [BINARY,JSON,XML].
	    @tparam T Array element value type.
	    @param name File name.
	    @param data target array.
	 */
    	template <Format F, typename T>
	void readFile(const String& name, MArray<T>& data);

	/** Serialize MArray and write to binary file.
	    @tparam T Array element value type.
	    @param name File name.
	    @param data Array to be saved.
	 */
	template <typename T>
	inline void writeBINARYFile(const String& name, const MArray<T>& data)
	{ writeFile<Format::BINARY>(name, data); }

	/** Read and deserialize MArray from cereal compatibel binary file.
	    @tparam T Array element value type.
	    @param name File name.
	    @param data target array.
	 */
    	template <typename T>
	inline void readBINARYFile(const String& name, MArray<T>& data)
	{ readFile<Format::BINARY>(name, data); }

	/** Serialize MArray and write to json file.
	    @tparam T Array element value type.
	    @param name File name.
	    @param data Array to be saved.
	 */
	template <typename T>
	inline void writeJSONFile(const String& name, const MArray<T>& data)
	{ writeFile<Format::JSON>(name, data); }

	/** Read and deserialize MArray from cereal compatibel json file.
	    @tparam T Array element value type.
	    @param name File name.
	    @param data target array.
	 */
    	template <typename T>
	inline void readJSONFile(const String& name, MArray<T>& data)
	{ readFile<Format::JSON>(name, data); }

	/** Serialize MArray and write to xml file.
	    @tparam T Array element value type.
	    @param name File name.
	    @param data Array to be saved.
	 */
	template <typename T>
	inline void writeXMLFile(const String& name, const MArray<T>& data)
	{ writeFile<Format::XML>(name, data); }

	/** Read and deserialize MArray from cereal compatibel xml file.
	    @tparam T Array element value type.
	    @param name File name.
	    @param data target array.
	 */
    	template <typename T>
	inline void readXMLFile(const String& name, MArray<T>& data)
	{ readFile<Format::XML>(name, data); }
    }
}

#endif
