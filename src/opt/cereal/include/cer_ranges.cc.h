// -*- C++ -*-
/**

   @file opt/cereal/include/cer_ranges.cc.h
   @brief Load and save implementation for cnorxz ranges.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_cereal_ranges_cc_h__
#define __cxz_cereal_ranges_cc_h__

#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

#include "base/base.h"
#include "ranges/crange.h"
#include "ranges/urange.h"
#include "ranges/yrange.h"

namespace CNORXZ
{
    /*============+
     |    save    |
     +============*/

    template <class Archive>
    void save(Archive& ar, const Uuid& id)
    {
	ar(CEREAL_NVP(id.i1));
	ar(CEREAL_NVP(id.i2));
    }
    
    template <class Archive>
    void CRange::save(Archive& ar, const std::uint32_t version) const
    {
	CXZ_ASSERT(version == 1u, "format version = " << version << " not supported");
	ar(cereal::make_nvp("uuid", RB::mId));
	ar(cereal::make_nvp("this", RB::mThis));
	ar(cereal::make_nvp("size", mSize));
    }

    template <class MetaT>
    template <class Archive>
    void URange<MetaT>::save(Archive& ar, const std::uint32_t version) const
    {
	CXZ_ASSERT(version == 1u, "format version = " << version << " not supported");
	ar(cereal::make_nvp("uuid", RB::mId));
	ar(cereal::make_nvp("this", RB::mThis));
	ar(cereal::make_nvp("meta", mSpace));
    }
    
    template <class Archive>
    void YRange::save(Archive& ar, const std::uint32_t version) const
    {
	CXZ_ASSERT(version == 1u, "format version = " << version << " not supported");
	ar(cereal::make_nvp("uuid", RB::mId));
	ar(cereal::make_nvp("this", RB::mThis));
	ar(cereal::make_nvp("sub", mRVec));
    }

    /*============+
     |    load    |
     +============*/
    
    template <class Archive>
    void load(Archive& ar, Uuid& id)
    {
	ar(CEREAL_NVP(id.i1));
	ar(CEREAL_NVP(id.i2));
    }

    template <class Archive>
    void CRange::load(Archive& ar, const std::uint32_t version)
    {
	CXZ_ASSERT(version == 1u, "format version = " << version << " not supported");
	ar(cereal::make_nvp("uuid", RB::mId));
	ar(cereal::make_nvp("this", RB::mThis));
	ar(cereal::make_nvp("size", mSize));
	CXZ_ASSERT(RangePtr(RB::mThis).get() == this, "got corrupted range data"); // yes, cereal is that awesome! :)
    }

    template <class MetaT>
    template <class Archive>
    void URange<MetaT>::load(Archive& ar, const std::uint32_t version)
    {
	CXZ_ASSERT(version == 1u, "format version = " << version << " not supported");
	ar(cereal::make_nvp("uuid", RB::mId));
	ar(cereal::make_nvp("this", RB::mThis));
	ar(cereal::make_nvp("meta", mSpace));
	CXZ_ASSERT(RangePtr(RB::mThis).get() == this, "got corrupted range data");
    }
    
    template <class Archive>
    void YRange::load(Archive& ar, const std::uint32_t version)
    {
	CXZ_ASSERT(version == 1u, "format version = " << version << " not supported");
	ar(cereal::make_nvp("uuid", RB::mId));
	ar(cereal::make_nvp("this", RB::mThis));
	ar(cereal::make_nvp("sub", mRVec));
	CXZ_ASSERT(RangePtr(RB::mThis).get() == this, "got corrupted range data");
	// register range:
	for(auto& r: mRVec){
	    r = RangeFactoryBase::getRegistered(r->type(), r);
	}
    }
}

#endif
