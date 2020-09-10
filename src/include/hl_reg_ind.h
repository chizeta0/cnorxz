
#ifndef __hl_reg_ind_h__
#define __hl_reg_ind_h__

#include "ranges/rheader.h"

namespace MultiArrayTools
{

    template <class Index>
    struct RegIndNum
    {
        static constexpr size_t VALUE = -1;
        static constexpr size_t DEPTH = 0;
    };

    template <>
    struct RegIndNum<ClassicRange::IndexType>
    {
        static constexpr size_t VALUE = 0;
        static constexpr size_t DEPTH = 3;
    };

    // to be returned by IndexWrapper
    struct RegIndInfo
    {
        size_t type;
        size_t depth;

        template <class Index>
        RegIndInfo& set(const std::shared_ptr<Index>& i)
        {
            type = RegIndNum<Index>::VALUE;
            depth = RegIndNum<Index>::DEPTH;
	    return *this;
        }
    };
}

#endif

#ifdef reg_ind1
#ifdef reg_ind2
#ifdef reg_ind3

reg_ind1(ClassicRange::IndexType);
reg_ind2(ClassicRange::IndexType,ClassicRange::IndexType);
reg_ind3(ClassicRange::IndexType,ClassicRange::IndexType,ClassicRange::IndexType);

#endif
#endif
#endif
