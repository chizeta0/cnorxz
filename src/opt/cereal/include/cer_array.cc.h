
#ifndef __cxz_cereal_array_cc_h__
#define __cxz_cereal_array_cc_h__

#include "base/base.h"
#include "array/marray.h"
#include <cereal/cereal.hpp>

namespace CNORXZ
{
    template <typename T>
    template <class Archive>
    void MArray<T>::save(Archive& ar) const
    {
	ar(cereal::make_nvp("range", AB::mRange));
	ar(cereal::make_nvp("data", mCont));
    }

    template <typename T>
    template <class Archive>
    void MArray<T>::load(Archive& ar)
    {
	ar(cereal::make_nvp("range", AB::mRange));
	ar(cereal::make_nvp("data", mCont));
    }
}

#endif
