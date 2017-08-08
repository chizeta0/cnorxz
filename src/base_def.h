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
    template <typename T, class Range>
    class MultiArrayBase;

    // multi_array.h
    template <typename T, class Range>
    class MutableMultiArrayBase;
    
    // multi_array.h
    template <typename T, class Range>
    class MultiArray;
    /*
    // multi_array_operation.h
    template <typename T>
    class MultiArrayOperationBase;

    // multi_array_operation.h
    template <typename T>
    class MutableMultiArrayOperationBase;

    // multi_array_operation.h
    template <typename T, class Range>
    class MultiArrayOperationRoot;

    // multi_array_operation.h
    template <typename T, class Range>
    class ConstMultiArrayOperationRoot;

    // multi_array_operation.h
    template <typename T, class InRange, class TotalInRange, class OutRange, class TotalRange>
    class MultiArrayOperationMap;
    
    // multi_array_operation.h
    template <typename T, class Operation, class... MAOps>
    class MultiArrayOperation;

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

#endif
