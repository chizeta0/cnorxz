
#ifndef __cxz_types_h__
#define __cxz_types_h__

#include <string>
#include <vector>
#include <memory>
#include <tuple>
#include <complex>
#include <array>
#include <map>
#include <typeinfo>
#include <utility>
#include <numeric>

namespace CNORXZ
{
    /**********************
     *   standard types   *
     **********************/

    typedef std::intptr_t PtrId;
    typedef int32_t Int;
    typedef int64_t LInt;
    typedef uint64_t SizeT;
    typedef double Double;
    typedef Double Real;
    typedef std::complex<Real> Complex;
    typedef std::string String;

    template <typename T, size_t N>
    using Arr = std::array<T,N>;
    
    template <typename T>
    using Sptr = std::shared_ptr<T>;

    template <typename T>
    using Wptr = std::weak_ptr<T>;

    template <typename T>
    using Uptr = std::unique_ptr<T>;

    template <typename T, typename U>
    using Pair = std::pair<T,U>;
    
    template <typename... T>
    using Tuple = std::tuple<T...>;

    template <SizeT I, typename... T>
    using TupleElem = std::tuple_element<I,Tuple<T...>>;

    template <typename T>
    using RemoveRef = typename std::remove_reference<T>::type;
    
    template <typename K, typename V>
    using Map = std::map<K,V>;

    typedef std::type_info TypeInfo;

    template <SizeT... Is>
    using Isq = std::index_sequence<Is...>;
    // cxz helper functions/classes: -> isq.h

    template <class... T>
    using Isqf = std::index_sequence_for<T...>;

    template <SizeT N>
    using CSizeT = std::integral_constant<SizeT,N>;
    
    /*********************
     *   library types   *
     *********************/

    /***
	Naming Prefixes:
	D = Y = Dynamic
	V = X = Virtual
	S = Static
	P = Partial = Sub
	C = Classic
	M = Multi (Index,Ranges) or !const (Container)
	U = One(=Uni) dimensional
	N = None = Null
	E = Extension (SSE,AVX,etc dof)
	T = Thread
	R = Rank
	A = (const) Array
	B = (mutable) Array
	F = Functional, Map,...
     ***/

    // default template parameter
    class None {};
    
    // definition: base/dtype.h
    class DType;

    // definition: base/obj_handle.h
    template <typename T>
    class ObjHandle;
    
    // definition: memory/allocator.h
    template <typename T>
    class Allocator;

    // definition: xpr/vpos_type.h
    class VPosBase;

    // definition: xpr/vpos_type.h
    template <class PosT>
    class VPos;

    // definition: xpr/vpos_type.h
    template <class PosT>
    class VPosRef;

    // definition: xpr/pos_type.h
    class UPos;

    // definition: xpr/pos_type.h
    template <SizeT N>
    class SPos;

    // definition: xpr/pos_type.h
    class FPos;

    // definition: xpr/pos_type.h
    template <SizeT N, SizeT... Ms>
    class SFPos;

    // definition: xpr/pos_type.h
    class DPos;

    // definition: xpr/pos_type.h
    class DPosRef;

    // definition: xpr/pos_type.h
    template <class PosT1, class PosT2>
    class MPos;

    // definition: ranges/range_base.h
    class RangeBase;

    typedef Sptr<RangeBase> RangePtr;
    
    // definition: ranges/index_base.h
    template <class I, typename MetaType>
    class IndexInterface;

    template <class I, typename MetaType>
    using IndexPtr = Sptr<IndexInterface<I,MetaType>>;

    // definition: ranges/nrange.h
    class NRange; // null-range (range over none)

    // definition: ranges/nrange.h
    class NIndex;
    
    // definition: ranges/urange.h
    template <typename Meta>
    class URange; // generic simple range (uni-dimensional)

    // definition: ranges/urange.h
    template <typename Meta>
    class UIndex;

    // definition: ranges/crange.h
    class CRange; // classic range, trivial meta data

    // definition: ranges/crange.h
    class CIndex;

    // definition: ranges/srange.h
    template <typename Meta, SizeT S>
    class SRange; // generic static size range

    // definition: ranges/srange.h
    template <typename Meta, SizeT S>
    class SIndex;

    // definition: ranges/mrange.h
    template <class... Ranges>
    class MRange; // multi range

    // definition: ranges/mrange.h
    template <class BlockType, class... Indices>
    class GMIndex;

    template <class... Indices>
    using MIndex = GMIndex<None,Indices...>;

    // definition: ranges/xindex.h
    class XIndexBase; // dynamic index wrapper

    typedef Sptr<XIndexBase> XIndexPtr;

    // definition: ranges/dindex.h
    class DIndex;
    
    // definition: ranges/yrange.h
    class YRange; // dynamic multi range
    
    // definition: ranges/yrange.h
    class YIndex;

    typedef Sptr<YIndex> YIndexPtr;
    
    // definition: ranges/pindex.h
    template <class Index>
    class PIndex; // partial index (index over sub-ranges and permutations)

    // there should be also a static analogue

    // definition: ranges/lindex.h
    template <class Index, SizeT L>
    class LIndex;

    // definition: ranges/index_pack.h
    template <class... Indices>
    class SPack;

    // definition: ranges/index_pack.h
    class DPack;

    // definition: ranges/index_format.h
    template <SizeT N>
    class MFormat;

    // definition: ranges/index_format.h
    template <class... PosT>
    class GMFormat;

    // definition: ranges/index_format.h
    class YFormat;
    
    // definition: array/array_base.h
    template <typename T>
    class CArrayBase;

    // definition: array/array_base.h
    template <typename T>
    class ArrayBase;

    // definition: array/array.h
    template <typename T>
    class MArray;

    // definition: array/slice.h
    template <typename T>
    class CSlice;

    // definition: array/slice.h
    template <typename T>
    class Slice;

    // definition: operation/op_types.h
    template <class OpT>
    class COpInterface;
    
    // definition: operation/op_types.h
    template <class OpT>
    class OpInterface;

    // definition: operation/op_types.h
    template <typename T, class IndexT>
    class COpRoot;

    // definition: operation/op_types.h
    template <typename T, class IndexT>
    class OpCont;

    // definition: operation/op_types.h
    template <typename T, class IndexT>
    class OpRoot;

    // definition: operation/op_types.h
    template <class F, class... Ops>
    class Operation;

    // definition: operation/op_types.h
    template <class CXpr>
    class Contraction;
    
    /*********************
     *   derived types   *
     *********************/
    
    template <typename T>
    using Vector = std::vector<T,Allocator<T>>;

    template <typename T, SizeT N, bool Static>
    struct Container
    { typedef Vector<T> type; };

    template <typename T, SizeT N>
    struct Container<T,N,true>
    { typedef Arr<T,N> type; };
}

#endif
