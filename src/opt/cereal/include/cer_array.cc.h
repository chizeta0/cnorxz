// -*- C++ -*-
/**

   @file opt/cereal/include/cer_array.cc.h
   @brief Load and save implementation for MArray.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __cxz_cereal_array_cc_h__
#define __cxz_cereal_array_cc_h__

#include "base/base.h"
#include "array/marray.h"
#include <cereal/cereal.hpp>

namespace CNORXZ
{
    template <typename T>
    template <class Archive>
    void MArray<T>::save(Archive& ar, const std::uint32_t version) const
    {
	CXZ_ASSERT(version == 1u, "format version = " << version << " not supported");
	ar(cereal::make_nvp("range", AB::mRange));
	ar(cereal::make_nvp("data", mCont));
    }

    template <typename T>
    template <class Archive>
    void MArray<T>::load(Archive& ar, const std::uint32_t version)
    {
	CXZ_ASSERT(version == 1u, "format version = " << version << " not supported");
	cer::save_load(ar, "range", AB::mRange);
	ar(cereal::make_nvp("data", mCont));
    }
}

#endif
