
#ifndef __cxz_cereal_type_register_cc_h__
#define __cxz_cereal_type_register_cc_h__

#include "cer_base.h"
#include "cer_header.h"
#include "cer_ranges.cc.h"
#include "cer_array.cc.h"
#include <cereal/types/polymorphic.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

CEREAL_CLASS_VERSION(CNORXZ::Uuid, CXZ_CEREAL_FORMAT_VERION);
CEREAL_CLASS_VERSION(CNORXZ::cer::Header, CXZ_CEREAL_FORMAT_VERION);

CEREAL_REGISTER_TYPE(CNORXZ::CRange);
CEREAL_REGISTER_POLYMORPHIC_RELATION(CNORXZ::RangeBase, CNORXZ::CRange);
CEREAL_CLASS_VERSION(CNORXZ::CRange, CXZ_CEREAL_FORMAT_VERION);

CEREAL_REGISTER_TYPE(CNORXZ::URange<CNORXZ::SizeT>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(CNORXZ::RangeBase, CNORXZ::URange<CNORXZ::SizeT>);
CEREAL_CLASS_VERSION(CNORXZ::URange<CNORXZ::SizeT>, CXZ_CEREAL_FORMAT_VERION);

CEREAL_REGISTER_TYPE(CNORXZ::URange<CNORXZ::Int>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(CNORXZ::RangeBase, CNORXZ::URange<CNORXZ::Int>);
CEREAL_CLASS_VERSION(CNORXZ::URange<CNORXZ::Int>, CXZ_CEREAL_FORMAT_VERION);

CEREAL_REGISTER_TYPE(CNORXZ::URange<CNORXZ::Double>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(CNORXZ::RangeBase, CNORXZ::URange<CNORXZ::Double>);
CEREAL_CLASS_VERSION(CNORXZ::URange<CNORXZ::Double>, CXZ_CEREAL_FORMAT_VERION);

CEREAL_REGISTER_TYPE(CNORXZ::URange<CNORXZ::String>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(CNORXZ::RangeBase, CNORXZ::URange<CNORXZ::String>);
CEREAL_CLASS_VERSION(CNORXZ::URange<CNORXZ::String>, CXZ_CEREAL_FORMAT_VERION);

CEREAL_REGISTER_TYPE(CNORXZ::URange<CNORXZ::Vector<CNORXZ::SizeT>>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(CNORXZ::RangeBase, CNORXZ::URange<CNORXZ::Vector<CNORXZ::SizeT>>);
CEREAL_CLASS_VERSION(CNORXZ::URange<CNORXZ::Vector<CNORXZ::SizeT>>, CXZ_CEREAL_FORMAT_VERION);

CEREAL_REGISTER_TYPE(CNORXZ::URange<CNORXZ::Vector<CNORXZ::Int>>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(CNORXZ::RangeBase, CNORXZ::URange<CNORXZ::Vector<CNORXZ::Int>>);
CEREAL_CLASS_VERSION(CNORXZ::URange<CNORXZ::Vector<CNORXZ::Int>>, CXZ_CEREAL_FORMAT_VERION);

CEREAL_REGISTER_TYPE(CNORXZ::URange<CNORXZ::Vector<CNORXZ::Double>>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(CNORXZ::RangeBase, CNORXZ::URange<CNORXZ::Vector<CNORXZ::Double>>);
CEREAL_CLASS_VERSION(CNORXZ::URange<CNORXZ::Vector<CNORXZ::Double>>, CXZ_CEREAL_FORMAT_VERION);

CEREAL_REGISTER_TYPE(CNORXZ::URange<CNORXZ::Vector<CNORXZ::String>>);
CEREAL_REGISTER_POLYMORPHIC_RELATION(CNORXZ::RangeBase, CNORXZ::URange<CNORXZ::Vector<CNORXZ::String>>);
CEREAL_CLASS_VERSION(CNORXZ::URange<CNORXZ::Vector<CNORXZ::String>>, CXZ_CEREAL_FORMAT_VERION);

CEREAL_REGISTER_TYPE(CNORXZ::YRange);
CEREAL_REGISTER_POLYMORPHIC_RELATION(CNORXZ::RangeBase, CNORXZ::YRange);
CEREAL_CLASS_VERSION(CNORXZ::YRange, CXZ_CEREAL_FORMAT_VERION);

#pragma GCC diagnostic pop

namespace cereal
{
    namespace detail
    {

	template <typename T>
	struct Version<CNORXZ::MArray<T>>
	{
	    static const std::uint32_t version;

	    static std::uint32_t registerVersion()
	    {
		::cereal::detail::StaticObject<Versions>::getInstance().mapping.emplace
		    ( std::type_index(typeid(CNORXZ::MArray<T>)).hash_code(), CXZ_CEREAL_FORMAT_VERION );
		return 3;
	    }
	    
	    static void unused() { (void)version; }
	};

	template <typename T>
	const std::uint32_t Version<CNORXZ::MArray<T>>::version =
	    Version<CNORXZ::MArray<T>>::registerVersion();
    }
}

#endif
