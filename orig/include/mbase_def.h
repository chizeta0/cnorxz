
#ifndef __cxz_mbase_def_h__
#define __cxz_mbase_def_h__

#include "ranges/rbase_def.h"
#include "allocator.h"

namespace CNORXZ
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

    // cxz_array.h
    template <typename T, class... SRanges>
    class ArrayBase;

    // cxz_array.h
    template <typename T, class... SRanges>
    class MutableArrayBase;
    
    // cxz_array.h
    template <typename T, class... SRanges>
    class Array;
   
    // cxz_operation.h
    template <typename T, class OperationClass>
    class OperationBase;

    // cxz_operation.h
    template <typename T, class OperationClass>
    class OperationTemplate;
    
    // cxz_operation.h
    template <typename T, class... Ranges>
    class OperationRoot;

    // cxz_operation.h
    template <typename T, class... Ranges>
    class ParallelOperationRoot;

    // cxz_operation.h
    template <typename T>
    class OperationValue;

    // cxz_operation.h
    template <typename T, class... Ranges>
    class ConstOperationRoot;

    // cxz_operation.h
    template <typename T, class Op>
    class OperationPointer;

    // cxz_operation.h
    template <typename T, class OpFunction, class... Ops>
    class Operation;

    // cxz_operation.h
    template <typename T, class Op, class IndexType>
    class Contraction;

    // cxz_operation.h
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