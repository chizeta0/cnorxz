// -*- C++ -*-

#ifndef __base_def_h__
#define __base_def_h__

#include <cassert>

#define DEBUG_MODE_X

#ifdef DEBUG_MODE_X

#include <iostream>
#define CHECK std::cout << __FILE__ << ": @" << __LINE__ << " in " << __func__ << std::endl;
#define VCHECK(a) std::cout << __FILE__ << ": @" << __LINE__ \
  << " in " << __func__ << ": " << #a << " = " << a << std::endl;

#else
#define CHECK
#define VCHECK(a)

#endif

#define DEFAULT_MEMBERS(__class_name__) __class_name__() = default; \
    __class_name__(const __class_name__& in) = default; \
    __class_name__& operator=(const __class_name__& in) = default; \
    __class_name__(__class_name__&& in) = default; \
    __class_name__& operator=(__class_name__&& in) = default

#define DEFAULT_MEMBERS_X(__class_name__) __class_name__(const __class_name__& in) = default; \
    __class_name__& operator=(const __class_name__& in) = default; \
    __class_name__(__class_name__&& in) = default; \
    __class_name__& operator=(__class_name__&& in) = default


namespace MultiArrayTools
{
    template <class MultiArray>
    void checkPtr(const MultiArray& in, bool check = false)
    {
	static MultiArray const* ptr = nullptr;
	if(not check){
	    ptr = &in;
	}
	else {
	    assert(ptr == &in);
	}
    }

    /***********************
     *  Provided Types     *
     ***********************/
    
    // range_base.h
    enum class RangeType;

    // range_base.h
    class RangeFactoryBase;
    
    // range_base.h
    class RangeBase;
    
    // range_base.h
    template <class Index>
    class RangeInterface;

    // index_base.h
    class IndexBase;

    // index_base.h
    template <typename MetaType>
    class IndexInterface;

    // single_range.h    
    template <typename U, RangeType TYPE>
    class SingleRange;

    // single_range.h    
    template <typename U, RangeType TYPE>
    class SingleRangeFactory;

    // single_range.h
    template <typename U, RangeType TYPE>
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

    /*
    // multi_array_operation.h
    template <typename T, class InRange, class TotalInRange, class OutRange, class TotalRange>
    class MultiArrayOperationMap;

    // multi_array_operation.h
    template <typename T, class ContractOperation, class Range, class... MAOps>
    class MultiArrayContraction;
    
    // slice.h
    template <typename T, class Range, class MARange>
    class Slice;

    // slice.h
    template <typename T, class Range, class MARange>
    class ConstSlice;

    // manipulator.h
    template <typename T>
    class ManipulatorBase;

    // manipulator.h
    template <typename T>
    class BinReader;

    // ma_functional.h
    template <class InRange, class OutRange>
    class IndexMapFunction;

    // ma_functional.h
    class vec3d2Function;

    // ma_functional.h
    class pyProjFunction;

    // ma_functional.h
    class pyProjNoSymFunction;
    */
}

namespace MultiArrayHelper
{
    // block.h
    template <typename T, class OpFunc, class BlockClass1, class BlockClass2>
    class BlockBinaryOp;
    
    // block.h
    enum class BlockType;
    
    // block.h
    template <typename T>
    class BlockBase;

    // block.h
    template <typename T>
    class MutableBlockBase;

    // block.h
    template <typename T>
    class Block;

    // block.h
    template <typename T>
    class MBlock;

    // block.h
    template <typename T>
    class BlockValue;

    // block.h
    template <typename T>
    class MBlockValue;

    // block.h
    template <typename T>
    class SplitBlock;

    // block.h
    template <typename T>
    class MSplitBlock;

    // block.h
    template <typename T>
    class BlockResult;

}

#endif
