
#ifndef __mbase_def_h__
#define __mbase_def_h__

#include "ranges/rbase_def.h"
#include "allocator.h"

namespace MultiArrayTools
{
    /***********************
     *  Provided Types     *
     ***********************/
    
    template <class... Ranges>
    using ContainerRangeFactory = MultiRangeFactory<Ranges...>;

    template <class... Ranges>
    using ContainerRange = MultiRange<Ranges...>;
    
    // container_index.h
    template <typename T, class... Indices>
    class ConstContainerIndex;

    // multi_array.h
    template <typename T, class... SRanges>
    class MultiArrayBase;

    // multi_array.h
    template <typename T, class... SRanges>
    class MutableMultiArrayBase;
    
    // multi_array.h
    template <typename T, class... SRanges>
    class MultiArray;
   
    // multi_array_operation.h
    template <typename T, class OperationClass>
    class OperationBase;

    // multi_array_operation.h
    template <typename T, class OperationClass>
    class OperationTemplate;
    
    // multi_array_operation.h
    template <typename T, class... Ranges>
    class OperationRoot;

    // multi_array_operation.h
    template <typename T, class... Ranges>
    class ParallelOperationRoot;

    // multi_array_operation.h
    template <typename T>
    class OperationValue;

    // multi_array_operation.h
    template <typename T, class... Ranges>
    class ConstOperationRoot;

    // multi_array_operation.h
    template <typename T, class Op>
    class OperationPointer;

    // multi_array_operation.h
    template <typename T, class OpFunction, class... Ops>
    class Operation;

    // multi_array_operation.h
    template <typename T, class Op, class IndexType>
    class Contraction;

    // multi_array_operation.h
    template <typename T, class Op, class... Indices>
    class SliceContraction;

    // slice.h
    template <typename T, class... SRanges>
    class Slice;

    template <typename T, class... SRanges>
    class ConstSlice;

    // slice.h
    template <typename T, class... SRanges>
    class SliceDef;

    // slice.h
    template <typename T, class... SRanges>
    class ConstSliceDef;

    // map_range.h
    template <class OITpye, class MapF, SpaceType XSTYPE, class... Indices>
    class GenMapIndex;
    
    // map_range.h
    template <class ORType, class MapF, SpaceType XSTYPE, class... Ranges>
    class GenMapRangeFactory;

    // map_range.h
    template <class ORType, class MapF, SpaceType XSTYPE, class... Ranges>
    class GenMapRange;
}

#endif
