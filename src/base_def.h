// -*- C++ -*-

#ifndef __base_def_h__
#define __base_def_h__

#define DEFAULT_MEMBERS(__class_name__) __class_name__() = default; \
    __class_name__(const __class_name__& in) = default; \
    __class_name__& operator=(const __class_name__& in) = default; \
    __class_name__(__class_name__&& in) = default; \
    __class_name__& operator=(__class_name__&& in) = default

namespace MultiArrayTools
{
    /***********************
     *  Provided Types     *
     ***********************/
    
    // range_base.h
    enum class RangeType;

    // range_base.h
    class MultiRangeType;

    // range_base.h
    template <class Index>
    class RangeBase;

    // range_base.h
    template <class Index>
    class SubRangeBase;

    // index_base.h
    class IndefinitIndexBase;

    // index_base.h
    template <class Index>
    class IndexBase;

    // single_range.h
    template <typename U, RangeType TYPE>
    class SingleIndex;

    // single_range.h    
    template <typename U, RangeType TYPE>
    class SingleRange;

    // multi_range.h
    template <class... Indices>
    class MultiIndex;

    // multi_range.h    
    template <class... Ranges>
    class MultiRange;
    
    // multi_array.h
    template <typename T, class Range>
    class MultiArray;

    // multi_array_operation.h
    template <typename T, class Range>
    class MultiArrayOperationBase;
    
    // multi_array_operation.h
    template <typename T, class Range, class Operation, class... Ranges>
    class MultiArrayOperation;
}

#endif
