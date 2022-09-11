
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

    template <typename... T>
    using Tuple = std::tuple<T...>;

    template <typename... T>
    using TupleElem = std::tuple_element<T...>;

    template <typename K, typename V>
    using Map = std::map<K,V>;

    typedef std::typeinfo TypeInfo;
    
    /*********************
     *   library types   *
     *********************/

    // definition: base/dtype.h
    class DType;

    // definition: memory/allocator.h
    template <typename T>
    class Allocator;

    // definition: ranges/range_base.h
    class RangeBase;

    typedef Sptr<RangeBase> RangePtr;
    
    // definition: ranges/index_base.h
    template <class I, typename MetaType>
    class IndexInterface;

    template <class I, typename MetaType>
    using IndexPtr = Sptr<IndexInterface<I,MetaType>>;
    
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

    // definition: ranges/yrange.h
    class YRange; // dynamic multi range
    
    // definition: ranges/yrange.h
    class YIndex;

    // definition: ranges/pindex.h
    template <class Index>
    class PIndex; // partial index (index over sub-ranges and permutations)

    // there should be also a static analogue
    
    /*********************
     *   derived types   *
     *********************/
    
    template <typename T>
    using Vector = std::vector<T,Allocator<T>>;
    
}

#endif
