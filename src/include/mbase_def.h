
#ifndef __mbase_def_h__
#define __mbase_def_h__

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