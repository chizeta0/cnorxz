// -*- C++ -*-

#ifndef __cxz_ranges_base_def_h__
#define __cxz_ranges_base_def_h__

#include "base/base.h"

#include "allocator.h"
#define MUI static_cast<size_t>(-1)

namespace CNORXZ
{

    template <class R>
    using SIZET = size_t;
    
    /***********************
     *  Provided Types     *
     ***********************/
    
    // range_base.h
    enum class SpaceType;

    // range_base.h
    class RangeFactoryBase;
    
    // range_base.h
    class RangeBase;
    
    // range_base.h
    template <class Index>
    class RangeInterface;

    // index_base.h
    template <class I, typename MetaType>
    class IndexInterface;

    // single_range.h    
    template <typename U, SpaceType TYPE, size_t S>
    class GenSingleRange;

    // single_range.h    
    template <typename U, SpaceType TYPE, size_t S>
    class GenSingleRangeFactory;

    // single_range.h
    template <typename U, SpaceType TYPE, size_t S>
    class GenSingleIndex;

    // subrange.h
    template <class Index>
    class SubIndex;

    // subrange.h
    template <class Range>
    class SubRangeFactory;

    // subrange.h
    template <class Range>
    class SubRange;
    
    // multi_range.h
    template <class... Ranges>
    class MultiRangeFactory;
    
    // multi_range.h    
    template <class... Ranges>
    class MultiRange;

    // multi_range.h
    template <class... Indices>
    class MultiIndex;

    // anonymous_range.h
    class AnonymousRangeFactory;

    // dynamic_range.h
    class IndexWrapperBase;

    // dynamic_range.h
    template <class Index>
    class IndexWrapper;

    // dynamic_range.h
    class DynamicIndex;
    
    // dynamic_range.h
    class DynamicRangeFactory;
    
    // dynamic_range.h
    class DynamicRange;

    // value_range.h
    template <typename U>
    class ValueRange;

    template <typename U>
    class ValueRangeFactory;

    template <typename U>
    class ValueIndex;
}

#endif
