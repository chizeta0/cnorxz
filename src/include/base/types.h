
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

namespace CNORXZ
{
    /**********************
     *   standard types   *
     **********************/

    typedef std::intptr_t PtrId;
    typedef int32_t Int;
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

    template <typename... T>
    using Tuple = std::tuple<T...>;

    template <SizeT I, typename... T>
    using TupleElem = std::tuple_element<I,Tuple<T...>>;

    template <typename K, typename V>
    using Map = std::map<K,V>;

    typedef std::type_info TypeInfo;
    
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
    
    // definition: base/dtype.h
    class DType;

    // definition: base/obj_handle.h
    template <typename T>
    class ObjHandle;
    
    // definition: memory/allocator.h
    template <typename T>
    class Allocator;

    // definition: ranges/xfor/pos_type.h
    template <class PosT>
    class CPosInterface;

    // definition: ranges/xfor/vpos_type.h
    class VPosBase;

    // definition: ranges/xfor/vpos_type.h
    template <class PosT>
    class VPos;

    // definition: ranges/xfor/vpos_type.h
    template <class PosT>
    class VPosRef;

    // definition: ranges/xfor/pos_type.h
    class UPos;

    // definition: ranges/xfor/pos_type.h
    template <SizeT N>
    class SPos;

    // definition: ranges/xfor/pos_type.h
    class FPos;

    // definition: ranges/xfor/pos_type.h
    template <SizeT N, SizeT... Ms>
    class SFPos;

    // definition: ranges/xfor/pos_type.h
    class DPos;

    // definition: ranges/xfor/pos_type.h
    class DPosRef;

    // definition: ranges/xfor/pos_type.h
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
    template <class... Indices>
    class MIndex;

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
    template <class Index>
    class LIndex;
    
    /*********************
     *   derived types   *
     *********************/
    
    template <typename T>
    using Vector = std::vector<T,Allocator<T>>;
    
}

#endif
