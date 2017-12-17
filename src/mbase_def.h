
#ifndef __mbase_def_h__
#define __mbase_def_h__

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
    class VirtualIndexWrapperBase;
    typedef VirtualIndexWrapperBase VIWB;    
    
    // index_base.h
    template <class I>
    class IndexWrapper;
    
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
    template <typename T>
    class OperationBase;

    // multi_array_operation.h
    template <typename T>
    class MutableOperationBase;

    // multi_array_operation.h
    template <typename T, class OperationClass>
    class OperationTemplate;
    
    // multi_array_operation.h
    template <typename T, class OpClass, class... Ranges>
    class OperationMaster;
    
    // multi_array_operation.h
    template <typename T, class... Ranges>
    class OperationRoot;

    // multi_array_operation.h
    template <typename T, class... Ranges>
    class ConstOperationRoot;

    // multi_array_operation.h
    template <typename T, class OpFunction, class... Ops>
    class Operation;

    // multi_array_operation.h
    template <typename T, class Op, class IndexType>
    class Contraction;
}

#endif
