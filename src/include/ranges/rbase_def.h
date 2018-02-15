// -*- C++ -*-

#ifndef __ranges_base_def_h__
#define __ranges_base_def_h__

namespace MultiArrayTools
{
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
    template <typename U, SpaceType TYPE>
    class SingleRange;

    // single_range.h    
    template <typename U, SpaceType TYPE>
    class SingleRangeFactory;

    // single_range.h
    template <typename U, SpaceType TYPE>
    class SingleIndex;
    
    // multi_range.h
    template <class... Ranges>
    class MultiRangeFactory;
    
    // multi_range.h    
    template <class... Ranges>
    class MultiRange;

    // multi_range.h
    template <class... Indices>
    class MultiIndex;

    // container_range.h
    template <class... Ranges>
    class ContainerRangeFactory;

    // container_range.h
    template <class... Ranges>
    class ContainerRange;

    // container_range.h
    template <class... Indices>
    class ContainerIndex;

    // anonymous_range.h
    class AnonymousRangeFactory;

    // anonymous_range.h
    class AnonymousRange;
}

#endif
