// -*- C++ -*-

#ifndef __ranges_base_def_h__
#define __ranges_base_def_h__

#include <cstdlib>

#include <iostream>
#ifndef CHECK
#define CHECK std::cout << __FILE__ << ": @" << __LINE__ << " in " << __func__ << std::endl;
#endif
#ifndef VCHECK
#define VCHECK(a) std::cout << __FILE__ << ": @" << __LINE__ \
  << " in " << __func__ << ": " << #a << " = " << a << std::endl;
#endif



namespace MultiArrayTools
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
    //template <typename T, class... Ranges>
    //class ContainerRangeFactory;
    template <typename T, class... Ranges>
    using ContainerRangeFactory = MultiRangeFactory<Ranges...>;

    // container_range.h
    //template <typename T, class... Ranges>
    //class ContainerRange;
    template <typename T, class... Ranges>
    using ContainerRange = MultiRange<Ranges...>;
    
    // container_range.h
    template <typename T, class... Indices>
    class ContainerIndex;

    // anonymous_range.h
    class AnonymousRangeFactory;

    // anonymous_range.h
    //class AnonymousRange;

    // dynamic_range.h
    template <class EC>
    class DynamicIndex;
    
    // dynamic_range.h
    template <class EC>
    class DynamicRangeFactory;
    
    // dynamic_range.h
    template <class EC>
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
