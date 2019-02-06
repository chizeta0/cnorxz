
#ifndef __mbase_def_h__
#define __mbase_def_h__

#include "ranges/rbase_def.h"

namespace MultiArrayTools
{
    /***********************
     *  Provided Types     *
     ***********************/
    
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
    //template <typename T>
    //class MutableOperationBase;

    // multi_array_operation.h
    template <typename T, class OperationClass>
    class OperationTemplate;
    
    // multi_array_operation.h
    template <typename T, class AOp, class OpClass, class... Ranges>
    class OperationMaster;
    
    // multi_array_operation.h
    template <typename T, class... Ranges>
    class OperationRoot;

    // multi_array_operation.h
    template <typename T>
    class OperationValue;

    // multi_array_operation.h
    template <typename T, class... Ranges>
    class ConstOperationRoot;

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
    template <class MapF, SpaceType XSTYPE, class... Indices>
    class GenMapIndex;
    
    // map_range.h
    template <class MapF, SpaceType XSTYPE, class... Ranges>
    class GenMapRangeFactory;

    // map_range.h
    template <class MapF, SpaceType XSTYPE, class... Ranges>
    class GenMapRange;
}

#endif
